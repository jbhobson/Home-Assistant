#pragma once

#include "esphome/core/component.h"
#include "esphome/core/preferences.h"

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/time/real_time_clock.h"

namespace esphome {
namespace gas_statistics {

using sensor::Sensor;

class GasStatistics : public Component {
 public:
  float get_setup_priority() const override { return setup_priority::DATA; }
  void dump_config() override;
  void setup() override;
  void loop() override;

  void reset_statistics();

  void set_time(time::RealTimeClock *time) { this->time_ = time; }
  void set_total(Sensor *sensor) { this->total_ = sensor; }

  void set_gas_today(Sensor *sensor) { this->gas_today_ = sensor; }
  void set_gas_yesterday(Sensor *sensor) { this->gas_yesterday_ = sensor; }
  void set_gas_week(Sensor *sensor) { this->gas_week_ = sensor; }
  void set_gas_month(Sensor *sensor) { this->gas_month_ = sensor; }
  void set_gas_year(Sensor *sensor) { this->gas_year_ = sensor; }

protected:
  uint32_t save_interval_{300};        // Save every 5min (adjust as needed, based on seconds)
  uint32_t last_save_time_{0};         // Timestamp of the last save
  uint32_t last_warning_time_{0};      // Timestamp of the last warning log

  ESPPreferenceObject pref_;
  time::RealTimeClock *time_;

  // input sensors
  Sensor *total_{nullptr};

  // exposed sensors
  Sensor *gas_today_{nullptr};
  Sensor *gas_yesterday_{nullptr};
  Sensor *gas_week_{nullptr};
  Sensor *gas_month_{nullptr};
  Sensor *gas_year_{nullptr};

  // Resetting state flag
  bool is_resetting_{false};
  // To prevent sensor updates
  bool prevent_sensor_update_{false};
  // Flag to wait for a valid sensor reading after reset
  bool waiting_for_sensor_read_{false};

  // start day of week configuration
  int gas_week_start_day_{2};
  // start day of month configuration
  int gas_month_start_day_{1};
  // start day of year configuration
  int gas_year_start_day_{1};

  struct gas_data_t {
    uint16_t current_day_of_year{0};
    float start_today{NAN};
    float start_yesterday{NAN};
    float start_week{NAN};
    float start_month{NAN};
    float start_year{NAN};

    // Add fields to store sensor values in globals
    float gas_today{NAN};
    float gas_yesterday{NAN};
    float gas_week{NAN};
    float gas_month{NAN};
    float gas_year{NAN};
  } gas_;

  void process_(float total);
  void save_();
};  // class GasStatistics

}  // namespace gas_statistics
}  // namespace esphome
