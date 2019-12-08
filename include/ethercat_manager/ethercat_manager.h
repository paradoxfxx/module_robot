#ifndef ETHERCAT_MANAGER_H
#define ETHERCAT_MANAGER_H

#include <stdexcept>
#include <string>

#include <stdint.h>

#include <boost/scoped_array.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/mutex.h>
// #include <alchemy/sem.h>
// #include <boilerplate/trace.h>
// #include <xenomai/init.h>
// #include <trank/rtdk.h>
// #include <osal.h>

namespace ethercat {

/**
 * \brief EtherCAT exception. Currently this is only thrown in the event
 *        of a failure to construct an EtherCat manager.
 */
class EtherCatError : public std::runtime_error
{
public:
    explicit EtherCatError(const std::string& what)
    : std::runtime_error(what)
    {}
};

    /**
    * \brief This class provides a CPP interface to the SimpleOpenEthercatMaster library
    * Given the name of an ethernet device, such as "eth0", it will connect,
    * start a thread that cycles data around the network, and provide read/write
    * access to the underlying io map.
    *
    * Please note that as used in these docs, 'Input' and 'Output' are relative to
    * your program. So the 'Output' registers are the ones you write to, for example.
    */
class EtherCatManager
{
    
public:
    /**
    * \brief Constructs and initializes the ethercat slaves on a given network interface.
    *
    * @param[in] ifname the name of the network interface that the ethercat chain 
    *                   is connected to (i.e. "eth0")
    *
    * Constructor can throw EtherCatError exception if SOEM could not be 
    * initialized.
    */
    EtherCatManager(const std::string& ifname, bool realtime = false);


    ~EtherCatManager();

    /**
    * \brief writes 'value' to the 'channel-th' output-register of the given 'slave'
    *  
    * @param[in] slave_no The slave number of the device to write to (>= 1)
    * @param[in] channel The byte offset into the output IOMap to write value to
    * @param[in] value The byte value to write
    *
    * This method currently makes no attempt to catch out of bounds errors. Make
    * sure you know your IOMap bounds.
    */
    void write(int slave_no, uint8_t channel, uint8_t value);

    /**
    * \brief Reads the "channel-th" input-register of the given slave no
    *  
    * @param[in] slave_no The slave number of the device to read from (>= 1)
    * @param[in] channel The byte offset into the input IOMap to read from
    */
    uint8_t readInput(int slave_no, uint8_t channel) const;

    /**
    * \brief Reads the "channel-th" output-register of the given slave no
    *  
    * @param[in] slave_no The slave number of the device to read from (>= 1)
    * @param[in] channel The byte offset into the output IOMap to read from
    */
    uint8_t readOutput(int slave_no, uint8_t channel) const;

    /**
    * \brief write the SDO object of the given slave no
    *
    * @param[in] slave_no The slave number of the device to read from (>= 1)
    * @param[in] index The index address of the parameter in SDO object
    * @param[in] subidx The sub-index address of the parameter in SDO object
    * @param[in] value value to write
    */
    template <typename T>
    uint8_t writeSDO(int slave_no, uint16_t index, uint8_t subidx, T value) const;

    /**
    * \brief read the SDO object of the given slave no
    *
    * @param[in] slave_no The slave number of the device to read from (>= 1)
    * @param[in] index The index address of the parameter in SDO object
    * @param[in] subidx The sub-index address of the parameter in SDO object
    */
    template <typename T>
    T readSDO(int slave_no, uint16_t index, uint8_t subidx) const;

    /**
    * \brief get the number of clients
    */
    int getNumClinets() const;

    // /**
    // * \brief init the DC time
    // */ 
    // void initDistributeClock();

    // /**
    // * \brief wait for next period.
    // */ 
    // void waitnextperiod();

    // /**
    // * \brief update the DC time
    // */ 
    // void updateDistributeClock();

    /*
        brief : 
     */
    bool ifRealtime() const;


    // void sent_receive_eth_msg();

private:

    bool initSoem(const std::string& ifname);


    const std::string ifname_;
    uint8_t iomap_[4096];
    int num_clients_;
    boost::thread cycle_thread_;
    mutable boost::mutex iomap_mutex_;
    bool stop_flag_;

    //for dc computation
    unsigned int cycle_ns;
    long long toff;
    long long cur_DCtime;
    long long max_DCtime;
    unsigned long long cur_dc32;
    unsigned long long pre_dc32;
    int32_t shift_time; //dc event shifted compared to master reference clock
    long long  diff_dc32;
    long ethercat_time;
    int wkc;

    bool real_time_;


};

}

#endif