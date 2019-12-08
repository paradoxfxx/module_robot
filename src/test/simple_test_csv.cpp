/** \file
 * \brief Example code for Simple Open EtherCAT master
 *
 * Usage : simple_test -i [ifname1]
 * ifname is NIC interface, f.e. eth0
 *
 * This is a minimal test.
 *
 */

#include <stdio.h>
#include <ethercat_manager/ethercat_manager.h>
#include <elmo_control/elmo_client.h>
#include <getopt.h>
#include <time.h>

static const int NSEC_PER_SECOND = 1e+9;
static const int USEC_PER_SECOND = 1e+6;

void timespecInc(struct timespec &tick, int nsec)
{
    tick.tv_nsec += nsec;
    while (tick.tv_nsec >= NSEC_PER_SECOND)
    {
        tick.tv_nsec -= NSEC_PER_SECOND;
        tick.tv_sec++;
    }
}

int main(int argc, char *argv[])
{
    int operation_mode = 0x09; //(csv)
    std::string ifname("enp4s0");
    printf("MINAS Simple Test using SOEM (Simple Open EtherCAT Master)\n");

  /* start slaveinfo */
  ethercat::EtherCatManager manager(ifname);
  std::vector<elmo_control::ElmoClient *> clients;
    for (int i = 0; i < manager.getNumClinets(); i++ )
    {
      clients.push_back(new elmo_control::ElmoClient(manager, i+1));
    }
  for (std::vector<elmo_control::ElmoClient*>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
      elmo_control::ElmoClient* client = (*it);
      // clear error
      client->reset();
      client->setInterpolationTimePeriod(1000);     // 1 msec
      // servo on
      client->servoOn();
      // get current positoin
      elmo_control::ElmoInput input = client->readInputs();
      int32 current_velocity = input.velocity_actual_value;

      // set target position
      elmo_control::ElmoOutput output;
      // set memory
      memset(&output, 0x00, sizeof(elmo_control::ElmoOutput));
      if ( operation_mode == 0x09 )
      { // (csv)
            output.target_velocity = current_velocity;
      }

      output.max_torque    = 500;    // 50% (unit 0.1%)
      output.controlword   = 0x001f; // move to operation enabled + new-set-point (bit4) + change set immediately

      output.operation_mode = operation_mode;
      client->writeOutputs(output);
      // operation mode specific
      while ( ! (input.statusword & 0x1000) ) {// bit12 (set-point-acknowledge)
          input = client->readInputs();
      }
      output.controlword   &= ~0x0010; // clear new-set-point (bit4)
      client->writeOutputs(output);

      printf("target velocity = %08x\n", output.target_velocity);

      // waiting until operation_mode is csv
      while(1){
        if(input.operation_mode != 0x09){
          input = client->readInputs();
          output.operation_mode = operation_mode;
          client->writeOutputs(output);
        }else break;
      }

    }

  //  nanoseconds
  double period = 3e+5;
  // get curren ttime
  struct timespec tick;
  clock_gettime(CLOCK_REALTIME, &tick);
  timespecInc(tick, period);


  for (int i = 0; i <= 20000; i++ ) {
      for (std::vector<elmo_control::ElmoClient*>::iterator it = clients.begin(); it != clients.end(); ++it)
        {
      elmo_control::ElmoClient* client = (*it);
      elmo_control::ElmoInput input = client->readInputs();
      elmo_control::ElmoOutput output = client->readOutputs();
      if ( i % 10 == 0)
      {
        printf(" ctrl %04x, status %04x, op_mode = %2d, pos = %08x, vel = %08x, tor = %08x\n",
          output.controlword, input.statusword, input.operation_mode, input.position_actual_value, input.velocity_actual_value, input.torque_actual_value);
        if ( input.statusword & 0x0400 ) { // target reached (bit 10)
          printf("target reached\n");
          break;
        }
      }

      output.controlword |= 0x0f;
      if ( input.operation_mode == 0x09 )
      {
      output.target_velocity =   0x2000;
      }
      client->writeOutputs(output);
      } // for clients


      timespecInc(tick, period);
      // check overrun
      struct timespec before;
      clock_gettime(CLOCK_REALTIME, &before);
      double overrun_time = (before.tv_sec + double(before.tv_nsec)/NSEC_PER_SECOND) -  (tick.tv_sec + double(tick.tv_nsec)/NSEC_PER_SECOND);
      if (overrun_time > 0.0)
        {
            fprintf(stderr, "  overrun: %f", overrun_time);
        }
      clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &tick, NULL);
  }

  for (std::vector<elmo_control::ElmoClient *>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
      elmo_control::ElmoClient* client = (*it);
      elmo_control::ElmoInput input = client->readInputs();
      client->printPDSStatus(input);
      client->printPDSOperation(input);
      client->servoOff();
    }

  printf("End program\n");

  return 0;
}