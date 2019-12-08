#ifndef ELMO_CLIENT_H
#define ELMO_CLIENT_H

#include <ethercat_manager/ethercat_manager.h>
#include <osal.h>
#include "common.h"


// Forward declaration of EtherCatManager
namespace ethercat
{
class EtherCatManager;
}

namespace elmo_control
{

/**
 * \brief This class provides a client for the EtherCAT manager object that
 *        can translate robot input/output messages and translate them to
 *        the underlying IO Map.
 */
typedef struct {
  // input
  uint32 position_actual_value;		      // 6064h : Position actual value
  uint32 position_follow_error_value;   //60f4
  uint16 torque_actual_value;		        // 6077h : Torque actual value
  uint16 statusword;			              // 6041h : Statusword
  uint8  operation_mode;		            // 6061h : Modes of operation display
  uint32 velocity_sensor_actual_value;  //6069
  uint32 velocity_actual_value;	  	    // 606Ch : Velocity actual value
  uint32 digital_inputs;	            	// 60FDh : Digital inputs
  uint16 analog_input;                  // 2205 : Analog input
  // uint32 current_actual_value;          // 6078  : Current actual
} ElmoInput;

typedef struct {
  uint32 target_position;		// 607Ah : Target Position
  uint32 target_velocity;		// 60FFh : Target Velocity
  uint16 target_torque;			// 6071h : Target Torque
  uint16 max_torque;			  // 6072h : Max Torque
  uint16 controlword;			  // 6040h : Controlword
  uint8  operation_mode;		// 6060h : Mode of operation
  uint32 position_offset;   //60b0
  uint32 velocity_offset;   //60B1
  uint32 torque_offset;     //60b2

} ElmoOutput;


class ElmoClient
{
public:
  /**
   * \brief Constructs a control interface to a MINUS AC Servo on
   *        the given ethercat network and the given slave_no.
   *
   * @param[in] manager The interface to an EtherCAT network that the gripper
   *                    is connected to.
   *
   * @param[in] slave_no The slave number of the gripper on the EtherCAT network
   *                     (>= 1)
   */
  ElmoClient(ethercat::EtherCatManager& manager, int slave_no);

  /**
   * \brief Write the given set of control flags to the memory of the controller
   *
   * @param[in] output The set of output-register values to write to the gripper
   */
  void writeOutputs(const ElmoOutput& output);

  /**
   * \brief Reads set of input-register values from the controller.
   * \return The AC servo input registers as read from the controller IOMap
   */
  ElmoInput readInputs() const;

  /**
   * \brief Reads set of output-register values from the controller.
   * \return The AC servo output registers as read from the controller IOMap
   */
  ElmoOutput readOutputs() const;

  /**
   * \brief Reset alarm
   * \return void
   */
  void reset();

  /**
   * \brief Send servo on sequence to the controller
   * \return void
   */
  void servoOn();

  /**
   * \brief Send servo off sequence to the controller
   * \return void
   */
  void servoOff();

  // /**
  //  * \brief initialize the distribute timer.
  //  * \return void
  //  */
  // void initDC();

  // /**
  //  * \brief update the distribute timer in loop.
  //  * \return void
  //  */
  // void updateDC();

  /*
   * \brief set Profile velocity 0 - 4294967295 (6081h / 00h)
   * \return void
   */
  void setProfileVelocity(uint32_t val);


  void setMaxVelocity(uint32_t val);

  /*
   * \brief get the absolute position encoder Pos by SDO 
   * \return uint32_t
   */
  uint32_t getAbsolutePos()const;

 /*
   * \brief get the absolute position encoder Vel by SDO 
   * \return uint32_t
   */
  uint32_t getAbsoluteVel()const;


  /*
   * \brief set motor rate torque  (6076h / 00h)
   * \return void
   */
  void setMotorRateTorque(uint32_t rate_torque);

  uint32_t readMotorRateTorque()const;

  void set_target_torque(uint);
  
  /*
   * \brief set Interpolation Time Period  1000, 2000, 4000 us
   * \return void
   */
  void setInterpolationTimePeriod(int us);

  int setInterpolationTimePeriod();

  /**
   * \brief print status from input data
   */
  void printPDSStatus(const ElmoInput input) const;

  /**
   * \brief print operation mode from input data
   */
  void printPDSOperation(const ElmoInput input) const;

  /**
   * \brief print control status from input data
   */
  void printPDSControl(const ElmoInput input) const;

  /**
   * \brief get status from input data
   * \return status
   */
  PDS_STATUS getPDSStatus(const ElmoInput input) const;

  /**
   * \brief get operation mode from input data
   * \return status
   */
  PDS_OPERATION getPDSOperation(const ElmoInput input) const;

  /**
   * \brief get control status from input data
   * \return status
   */
  PDS_STATUS getPDSControl(const ElmoInput input) const;

   /**
   * \brief read the interpolation buff actual size
   * \return uint8
   */
  uint8 readTrajBuffSize() const;

  /**
   * \brief set the maxmuim buff size of interpolation.
   * \return int
   */
  int setInterpolationBuffSize();

private:


  ethercat::EtherCatManager& manager_;

  const int slave_no_;

  int interpolationTimePeriod;



};

/**
 * \brief Table of error code and text
 */
const struct {
  unsigned int code;
  const char* text;
} error_map[] = {
  {11, "Control power supply under-voltage protection"},
  {12, "Over voltage protection"},
  {13, "Main power supply under-voltage protection(between P and N)"},
  {14, "Over-current protection"}, 
  {15, "Over-heat protection"},
  {16, "Over-load protection"},
  {18, "Over-regeneration load protection"},
  {21, "Encoder communication disconnect error protection"},
  {23, "Encoder communication data error protection"},
  {24, "Position deviation excess protection"},
  {25, "Hybrid deviation excess error protection"},
  {26, "Over speed protection"},
  {27, "Command pulse input frequency error protection"},
  {28, "Limit of pulse replay error protection"},
  {29, "Deviation counter overflow protection"},
  {30, "Safety detection"},
  {33, "IF overlaps allocation error 1 protection"},
  {34, "Software limit protection"},
  {36, "EEPROM parameter error protection"},
  {37, "EEPROM check code error protection"},
  {38, "Over-travel inhibit input protection"},
  {39, "Analog input1 excess protection"},
  {40, "Absolute system down error protection"},
  {41, "Absolute counter over error protection"},
  {42, "Absolute over-speed error protection"},
  {43, "Initialization failure"},
  {44, "Absolute single turn counter error protection"},
  {45, "Absolute multi-turn counter error protection"},
  {47, "Absolute status error protection"},
  {48, "Encoder Z-phase error protection"},
  {49, "Encoder CS signal error protection"},
  {50, "Feedback scale connection error protection"},
  {51, "Feedback scale status 0 error protection"},
  {55, "A-phase connection error protection"},
  {87, "Compulsory alarm input protection"},
  {95, "Motor automatic recognition error protection"},
  {99, "Other error"},
};

}
#endif
