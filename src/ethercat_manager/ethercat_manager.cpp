#include <ethercat_manager/ethercat_manager.h>
// #include <ethercat_manager/ecat_dc.h>

#include <unistd.h>
#include <stdio.h>
// #include <time.h>

// #include <boost/ref.hpp>
// #include <boost/interprocess/sync/scoped_lock.hpp>

#include <ethercattype.h>
#include <nicdrv.h>
#include <ethercatbase.h>
#include <ethercatmain.h>
#include <ethercatdc.h>
#include <ethercatcoe.h>
#include <ethercatfoe.h>
#include <ethercatconfig.h>
#include <ethercatprint.h>

namespace
{
static const unsigned EC_TIMEOUTMON = 500;

static const RTIME period_xenomai = 900000;  /*900us */
static const RTIME timeout = 1000000; /*1 ms */

RT_MUTEX mutex_;
RT_TASK task;

void handleErrors()
{
  /* one ore more slaves are not responding */
  ec_group[0].docheckstate = FALSE;
  ec_readstate();
  for (int slave = 1; slave <= ec_slavecount; slave++)
  {
    if ((ec_slave[slave].group == 0) && (ec_slave[slave].state != EC_STATE_OPERATIONAL))
    {
      ec_group[0].docheckstate = TRUE;
      if (ec_slave[slave].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
      {
        fprintf(stderr, "ERROR : slave %d is in SAFE_OP + ERROR, attempting ack.\n", slave);
        ec_slave[slave].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
        ec_writestate(slave);
      }
      else if(ec_slave[slave].state == EC_STATE_SAFE_OP)
      {
        fprintf(stderr, "WARNING : slave %d is in SAFE_OP, change to OPERATIONAL.\n", slave);
        ec_slave[slave].state = EC_STATE_OPERATIONAL;
        ec_writestate(slave);
      }
      else if(ec_slave[slave].state > 0)
      {
        if (ec_reconfig_slave(slave, EC_TIMEOUTMON))
        {
          ec_slave[slave].islost = FALSE;
          printf("MESSAGE : slave %d reconfigured\n",slave);
        }
      }
      else if(!ec_slave[slave].islost)
      {
        /* re-check state */
        ec_statecheck(slave, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
        if (!ec_slave[slave].state)
        {
          ec_slave[slave].islost = TRUE;
          fprintf(stderr, "ERROR : slave %d lost\n",slave);
          throw std::runtime_error("ERROR : slave lost");
        }
      }
    }
    if (ec_slave[slave].islost)
    {
      if(!ec_slave[slave].state)
      {
        if (ec_recover_slave(slave, EC_TIMEOUTMON))
        {
          ec_slave[slave].islost = FALSE;
          printf("MESSAGE : slave %d recovered\n",slave);
        }
      }
      else
      {
        ec_slave[slave].islost = FALSE;
        printf("MESSAGE : slave %d found\n",slave);
      }
    }
  }
}


void cycleWorker_xenomai(void *)
{

        
  	rt_task_set_periodic(NULL, TM_NOW, period_xenomai);

    int expected_wkc;
    int sent, wkc;

    while(1)
	{
      
		rt_task_wait_period(NULL);
		rt_mutex_acquire(&mutex_,timeout);

		sent = ec_send_processdata();
		wkc = ec_receive_processdata(EC_TIMEOUTRET);

		rt_mutex_release(&mutex_);

		expected_wkc = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
		if (wkc < expected_wkc)
		{
			handleErrors();
		}  
  	}
}

} // end of anonymous namespace


namespace ethercat {

EtherCatManager::EtherCatManager(const std::string& ifname)
  : ifname_(ifname),
    num_clients_(0)
{
  if (initSoem(ifname))
  {


	rt_mutex_create(&mutex_,"cyclerWorker_mutex");
	rt_task_create(&task, "cycleWorker_xenomai", 0, THREAD_ETHERCAT_PRIORITY, 0);
	rt_task_start(&task, &cycleWorker_xenomai, (void *)NULL);



  }
  else
 {
   // construction failed
   throw EtherCatError("Could not initialize SOEM");
 }
}

EtherCatManager::~EtherCatManager()
{
	

	// Request init operational state for all slaves
	ec_slave[0].state = EC_STATE_INIT;

	/* request init state for all slaves */
	ec_writestate(0);

	//stop SOEM, close socket
	ec_close();

	rt_mutex_delete(&mutex_);
	rt_task_delete(&task);

}

#define IF_ELMO(_ec_slave) 1
bool EtherCatManager::initSoem(const std::string& ifname) {
	// Copy string contents because SOEM library doesn't
	// practice const correctness
	const static unsigned MAX_BUFF_SIZE = 1024;
	char buffer[MAX_BUFF_SIZE];
	size_t name_size = ifname_.size();
	if (name_size > sizeof(buffer) - 1)
	{
		fprintf(stderr, "Ifname %s exceeds maximum size of %u bytes\n", ifname_.c_str(), MAX_BUFF_SIZE);
		return false;
	}
	std::strncpy(buffer, ifname_.c_str(), MAX_BUFF_SIZE);

	printf("Initializing etherCAT master\n");

	if (!ec_init(buffer))
	{
		fprintf(stderr, "Could not initialize ethercat driver\n");
		return false;
	}

	/* find and auto-config slaves */
	if (ec_config_init(FALSE) <= 0)
	{
		fprintf(stderr, "No slaves found on %s\n", ifname_.c_str());
		return false;
	}

	printf("SOEM found and configured %d slaves\n", ec_slavecount);
	for( int cnt = 1 ; cnt <= ec_slavecount ; cnt++)
	{
		// ELMO Serial Man = 066Fh, ID = [5/D]****[0/4/8][0-F]*
		printf(" Man: %8.8x ID: %8.8x Rev: %8.8x %s\n", (int)ec_slave[cnt].eep_man, (int)ec_slave[cnt].eep_id, (int)ec_slave[cnt].eep_rev, IF_ELMO(ec_slave[cnt])?" ELMO Drivers":"");
		if(IF_ELMO(ec_slave[cnt])) {
			num_clients_++;
		}
	}
	printf("Found %d ELMO Drivers\n", num_clients_);


	if (ec_statecheck(0, EC_STATE_PRE_OP, EC_TIMEOUTSTATE*4) != EC_STATE_PRE_OP)
	{
		fprintf(stderr, "Could not set EC_STATE_PRE_OP\n");
		return false;
	}

	// use PDO mapping 4
	for( int cnt = 1 ; cnt <= ec_slavecount ; cnt++)
	{
		if (! IF_ELMO(ec_slave[cnt])) continue;
		int ret = 0, l;
		uint8_t num_pdo ;
		//uint8_t num_entries;
		// set 0 change PDO mapping index
		num_pdo = 0;
		ret += ec_SDOwrite(cnt, 0x1c12, 0x00, FALSE, sizeof(num_pdo), &num_pdo, EC_TIMEOUTRXM);
		// set to default PDO mapping 1

		uint16_t idx_rxpdo = 0x1605;
		ret += ec_SDOwrite(cnt, 0x1c12, 0x01, FALSE, sizeof(idx_rxpdo), &idx_rxpdo, EC_TIMEOUTRXM);
		idx_rxpdo = 0x1616;
		ret += ec_SDOwrite(cnt, 0x1c12, 0x02, FALSE, sizeof(idx_rxpdo), &idx_rxpdo, EC_TIMEOUTRXM);
		idx_rxpdo = 0x1617;
		ret += ec_SDOwrite(cnt, 0x1c12, 0x03, FALSE, sizeof(idx_rxpdo), &idx_rxpdo, EC_TIMEOUTRXM);
		idx_rxpdo = 0x1618;
		ret += ec_SDOwrite(cnt, 0x1c12, 0x04, FALSE, sizeof(idx_rxpdo), &idx_rxpdo, EC_TIMEOUTRXM);           
		// set number of assigned PDOs
		num_pdo = 4;
		ret += ec_SDOwrite(cnt, 0x1c12, 0x00, FALSE, sizeof(num_pdo), &num_pdo, EC_TIMEOUTRXM);
		printf("RxPDO mapping object index %d = %04x ret=%d\n", cnt, idx_rxpdo, ret);


		// set 0 change PDO mapping index
		num_pdo = 0;
		ret += ec_SDOwrite(cnt, 0x1c13, 0x00, FALSE, sizeof(num_pdo), &num_pdo, EC_TIMEOUTRXM);
		// set to default PDO mapping 1
		uint16_t idx_txpdo = 0x1a04;
		ret += ec_SDOwrite(cnt, 0x1c13, 0x01, FALSE, sizeof(idx_txpdo), &idx_txpdo, EC_TIMEOUTRXM);
		idx_txpdo = 0x1a0f;
		ret += ec_SDOwrite(cnt, 0x1c13, 0x02, FALSE, sizeof(idx_txpdo), &idx_txpdo, EC_TIMEOUTRXM);
		idx_txpdo = 0x1a11;
		ret += ec_SDOwrite(cnt, 0x1c13, 0x03, FALSE, sizeof(idx_txpdo), &idx_txpdo, EC_TIMEOUTRXM);
		idx_txpdo = 0x1a1c;
		ret += ec_SDOwrite(cnt, 0x1c13, 0x04, FALSE, sizeof(idx_txpdo), &idx_txpdo, EC_TIMEOUTRXM);
		idx_txpdo = 0x1a1d;
		ret += ec_SDOwrite(cnt, 0x1c13, 0x06, FALSE, sizeof(idx_txpdo), &idx_txpdo, EC_TIMEOUTRXM);
		idx_txpdo = 0x1a1f;
		ret += ec_SDOwrite(cnt, 0x1c13, 0x05, FALSE, sizeof(idx_txpdo), &idx_txpdo, EC_TIMEOUTRXM);
		
		// set number of assigned PDOs
		num_pdo = 6;
		ret += ec_SDOwrite(cnt, 0x1c13, 0x00, FALSE, sizeof(num_pdo), &num_pdo, EC_TIMEOUTRXM);
		printf("TxPDO mapping object index %d = %04x ret=%d\n", cnt, idx_txpdo, ret);
	}

	// configure IOMap
	int iomap_size = ec_config_map(iomap_);
	printf("SOEM IOMap size: %d\n", iomap_size);

	// locates dc slaves
	if(ec_configdc()){
		printf("configure DC success.\n");
	}else{
		printf("configure DC error. \n");
	}

	// '0' here addresses all slaves
	if (ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE*4) != EC_STATE_SAFE_OP)
	{
		fprintf(stderr, "Could not set EC_STATE_SAFE_OP\n");
		return false;
	}

	/*
		This section attempts to bring all slaves to operational status. It does so
		by attempting to set the status of all slaves (ec_slave[0]) to operational,
		then proceeding through 40 send/recieve cycles each waiting up to 50 ms for a
		response about the status.
	*/
	ec_slave[0].state = EC_STATE_OPERATIONAL;
	ec_send_processdata();
	ec_receive_processdata(4*EC_TIMEOUTRET);
	ec_writestate(0);

	int chk = 40;
	do {
		ec_send_processdata();
		ec_receive_processdata(EC_TIMEOUTRET);
		ec_statecheck(0, EC_STATE_OPERATIONAL, 50000); // 50 ms wait for state check
	} while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));

	if(ec_statecheck(0,EC_STATE_OPERATIONAL, EC_TIMEOUTSTATE) != EC_STATE_OPERATIONAL)
	{
		fprintf(stderr, "OPERATIONAL state not set, exiting\n");
		return false;
	}

	ec_readstate();
	for( int cnt = 1 ; cnt <= ec_slavecount ; cnt++)
	{
		if (! IF_ELMO(ec_slave[cnt])) continue;
		printf("\nSlave:%d\n Name:%s\n Output size: %dbits\n Input size: %dbits\n State: %d\n Delay: %d[ns]\n Has DC: %d\n",
			cnt, ec_slave[cnt].name, ec_slave[cnt].Obits, ec_slave[cnt].Ibits,
			ec_slave[cnt].state, ec_slave[cnt].pdelay, ec_slave[cnt].hasdc);
		if (ec_slave[cnt].hasdc) printf(" DCParentport:%d\n", ec_slave[cnt].parentport);
		printf(" Activeports:%d.%d.%d.%d\n", (ec_slave[cnt].activeports & 0x01) > 0 ,
			(ec_slave[cnt].activeports & 0x02) > 0 ,
			(ec_slave[cnt].activeports & 0x04) > 0 ,
			(ec_slave[cnt].activeports & 0x08) > 0 );
		printf(" Configured address: %4.4x\n", ec_slave[cnt].configadr);
	}

	for( int cnt = 1 ; cnt <= ec_slavecount ; cnt++)
	{
		if (! IF_ELMO(ec_slave[cnt])) continue;
		int ret = 0, l;
		uint16_t sync_mode;
		uint32_t cycle_time;
		uint32_t minimum_cycle_time;
		uint32_t sync0_cycle_time;
		l = sizeof(sync_mode);
		ret += ec_SDOread(cnt, 0x1c32, 0x01, FALSE, &l, &sync_mode, EC_TIMEOUTRXM);
		l = sizeof(cycle_time);
		ret += ec_SDOread(cnt, 0x1c32, 0x02, FALSE, &l, &cycle_time, EC_TIMEOUTRXM);
		l = sizeof(minimum_cycle_time);
		ret += ec_SDOread(cnt, 0x1c32, 0x05, FALSE, &l, &minimum_cycle_time, EC_TIMEOUTRXM);
		l = sizeof(sync0_cycle_time);
		ret += ec_SDOread(cnt, 0x1c32, 0x0a, FALSE, &l, &sync0_cycle_time, EC_TIMEOUTRXM);
		printf("PDO syncmode %02x, cycle time %d ns (min %d), sync0 cycle time %d ns, ret = %d\n", sync_mode, cycle_time, minimum_cycle_time, sync0_cycle_time, ret);
	}

	printf("\nFinished configuration successfully\n");
	return true;
}

void EtherCatManager::write(int slave_no, uint8_t channel, uint8_t value)
{
	if(!real_time_){
		boost::mutex::scoped_lock lock(iomap_mutex_);
		ec_slave[slave_no].outputs[channel] = value;
	}else{
		rt_mutex_acquire(&mutex_,timeout);
		ec_slave[slave_no].outputs[channel] = value;
		rt_mutex_release(&mutex_);
	}
}

uint8_t EtherCatManager::readInput(int slave_no, uint8_t channel) const
{
	if(!real_time_){
		boost::mutex::scoped_lock lock(iomap_mutex_);
		if (slave_no > ec_slavecount) {
			fprintf(stderr, "ERROR : slave_no(%d) is larger than ec_slavecount(%d)\n", slave_no, ec_slavecount);
			exit(1);
		}
		if (channel*8 >= ec_slave[slave_no].Ibits) {
			fprintf(stderr, "ERROR : channel(%d) is larget thatn Input bits (%d)\n", channel*8, ec_slave[slave_no].Ibits);
			exit(1);
		}
	}else{
		rt_mutex_acquire(&mutex_,timeout);
		if (slave_no > ec_slavecount) {
			fprintf(stderr, "ERROR : slave_no(%d) is larger than ec_slavecount(%d)\n", slave_no, ec_slavecount);
			rt_mutex_release(&mutex_);
			exit(1);
		}
		if (channel*8 >= ec_slave[slave_no].Ibits) {
			fprintf(stderr, "ERROR : channel(%d) is larget thatn Input bits (%d)\n", channel*8, ec_slave[slave_no].Ibits);
			rt_mutex_release(&mutex_);
			exit(1);
		}
		rt_mutex_release(&mutex_);	
	}
	return ec_slave[slave_no].inputs[channel];
}

uint8_t EtherCatManager::readOutput(int slave_no, uint8_t channel) const
{
  	
	if(!real_time_){
		boost::mutex::scoped_lock lock(iomap_mutex_);
		if (slave_no > ec_slavecount) {
			fprintf(stderr, "ERROR : slave_no(%d) is larger than ec_slavecount(%d)\n", slave_no, ec_slavecount);
			exit(1);
		}
		if (channel*8 >= ec_slave[slave_no].Obits) {
			fprintf(stderr, "ERROR : channel(%d) is larget thatn Output bits (%d)\n", channel*8, ec_slave[slave_no].Obits);
			exit(1);
		}
	}else{

		rt_mutex_acquire(&mutex_,timeout);
		if (slave_no > ec_slavecount) {
			fprintf(stderr, "ERROR : slave_no(%d) is larger than ec_slavecount(%d)\n", slave_no, ec_slavecount);
			rt_mutex_release(&mutex_);
			exit(1);
		}
		if (channel*8 >= ec_slave[slave_no].Obits) {
			fprintf(stderr, "ERROR : channel(%d) is larget thatn Output bits (%d)\n", channel*8, ec_slave[slave_no].Obits);
			rt_mutex_release(&mutex_);
			exit(1);
		}
		rt_mutex_release(&mutex_);

	}
  return ec_slave[slave_no].outputs[channel];
}

template <typename T>
uint8_t EtherCatManager::writeSDO(int slave_no, uint16_t index, uint8_t subidx, T value) const
{	
	int ret;
	if(!real_time_){
		boost::mutex::scoped_lock lock(iomap_mutex_);	
		ret = ec_SDOwrite(slave_no, index, subidx, FALSE, sizeof(value), &value, EC_TIMEOUTSAFE);
	}else{
		rt_mutex_acquire(&mutex_,timeout);
		ret = ec_SDOwrite(slave_no, index, subidx, FALSE, sizeof(value), &value, EC_TIMEOUTSAFE);
		rt_mutex_release(&mutex_);
	}
		return ret;
}

template <typename T>
T EtherCatManager::readSDO(int slave_no, uint16_t index, uint8_t subidx) const
{
	int ret, l;
	T val;
	l = sizeof(val);
	if(!real_time_){
		boost::mutex::scoped_lock lock(iomap_mutex_);	
		ret = ec_SDOread(slave_no, index, subidx, FALSE, &l, &val, EC_TIMEOUTRXM);
	}else{
		rt_mutex_acquire(&mutex_,timeout);
		ret = ec_SDOread(slave_no, index, subidx, FALSE, &l, &val, EC_TIMEOUTRXM);
		rt_mutex_release(&mutex_);
	}
	if ( ret <= 0 ) { // ret = Workcounter from last slave response
		fprintf(stderr, "Failed to read from ret:%d, slave_no:%d, index:0x%04x, subidx:0x%02x\n", ret, slave_no, index, subidx);
	}
	return val;
}

int EtherCatManager::getNumClinets() const
{
	return num_clients_;
}

template uint8_t EtherCatManager::writeSDO<char> (int slave_no, uint16_t index, uint8_t subidx, char value) const;
template uint8_t EtherCatManager::writeSDO<int> (int slave_no, uint16_t index, uint8_t subidx, int value) const;
template uint8_t EtherCatManager::writeSDO<short> (int slave_no, uint16_t index, uint8_t subidx, short value) const;
template uint8_t EtherCatManager::writeSDO<long> (int slave_no, uint16_t index, uint8_t subidx, long value) const;
template uint8_t EtherCatManager::writeSDO<unsigned char> (int slave_no, uint16_t index, uint8_t subidx, unsigned char value) const;
template uint8_t EtherCatManager::writeSDO<unsigned int> (int slave_no, uint16_t index, uint8_t subidx, unsigned int value) const;
template uint8_t EtherCatManager::writeSDO<unsigned short> (int slave_no, uint16_t index, uint8_t subidx, unsigned short value) const;
template uint8_t EtherCatManager::writeSDO<unsigned long> (int slave_no, uint16_t index, uint8_t subidx, unsigned long value) const;

template char EtherCatManager::readSDO<char> (int slave_no, uint16_t index, uint8_t subidx) const;
template int EtherCatManager::readSDO<int> (int slave_no, uint16_t index, uint8_t subidx) const;
template short EtherCatManager::readSDO<short> (int slave_no, uint16_t index, uint8_t subidx) const;
template long EtherCatManager::readSDO<long> (int slave_no, uint16_t index, uint8_t subidx) const;
template unsigned char EtherCatManager::readSDO<unsigned char> (int slave_no, uint16_t index, uint8_t subidx) const;
template unsigned int EtherCatManager::readSDO<unsigned int> (int slave_no, uint16_t index, uint8_t subidx) const;
template unsigned short EtherCatManager::readSDO<unsigned short> (int slave_no, uint16_t index, uint8_t subidx) const;
template unsigned long EtherCatManager::readSDO<unsigned long> (int slave_no, uint16_t index, uint8_t subidx) const;

}