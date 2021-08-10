#include "vm_config.h"
#include <stdint.h>
#include "mrubyc.h"
#include "thermostat.h"
#include <stdio.h>

typedef struct th_day {
	uint8_t day;
	uint8_t night;
} th_day_t;

typedef struct th_calendar {
	th_day_t *pCal;
} th_calendar_t;

//================================================================
/*! write
  s = $therm.write(t, d, v)
  @param  t		day or night; day == 0, night = 1
  @param  d		day of the week
  @param  d		value
  @return Int temperature
*/
static void c_thermostat_write(mrbc_vm *vm, mrbc_value v[], int argc) {
	if (argc == 3) {
		int day_night = GET_INT_ARG(1);
		uint8_t day_of_week = GET_INT_ARG(2);
		int value = GET_INT_ARG(3);
		uint8_t idx = (uint8_t)day_of_week;

		th_calendar_t *cal = (th_calendar_t *)v->instance->data;
		th_day_t *days = cal->pCal;
		if (day_night) {
			days[idx].night = value;
		} else {
			days[idx].day = value;
		}
	}
	SET_INT_RETURN(0);
}


//================================================================
/*! read
  s = $therm.read(t, d)
  @param  t		day or night; day == 0, night = 1
  @param  d		day of the week
  @return Int temperature
*/
static void c_thermostat_read(mrbc_vm *vm, mrbc_value v[], int argc) {
  if (argc != 2) {
	  SET_INT_RETURN(0);
	  return;
  }
  int day_night = GET_INT_ARG(1);
  int day_of_week = GET_INT_ARG(2);
  int idx = day_of_week;

  th_calendar_t *cal = (th_calendar_t *)v->instance->data;
  th_day_t *days = cal->pCal;
  th_day_t day = days[idx];
  if (day_night) {
	  SET_INT_RETURN(day.night);
  } else {
	  SET_INT_RETURN(day.day);
  }
}


//================================================================
/*! Thermostat constructor
  $therm = Thermostat.new	# new thermostat
*/
static void c_thermostat_new(mrbc_vm *vm, mrbc_value v[], int argc) {
  *v = mrbc_instance_new(vm, v->cls, sizeof(th_calendar_t)); // calendar
  th_day_t *days = mrbc_raw_alloc_no_free(sizeof(th_day_t) * 7); // all days
  th_calendar_t *cal = (th_calendar_t *)v->instance->data;
  cal->pCal = days;
  //memcpy(v->instance->data[0], &d, sizeof(intptr_t));
  for (int i = 0; i < 7; i++) {
	  if (i == 0) {
		  days[i].day = 75;
		  days[i].night = 67;
	  } else {
		  days[i].day = 78;
		  days[i].night = 69;
	  }
  }
  return;
}

//================================================================
/*! initialize
*/
void mrbc_init_class_thermostat(struct VM *vm) {
  // define class and methods.
  mrbc_class *thermostat;
  thermostat = mrbc_define_class(vm, "Thermostat",	mrbc_class_object);
  mrbc_define_method(vm, thermostat, "new",	c_thermostat_new);
  mrbc_define_method(vm, thermostat, "read",	c_thermostat_read);
  mrbc_define_method(vm, thermostat, "write",	c_thermostat_write);
  //mrbc_define_method(0, thermostat, "transfer",c_spi_transfer);
}
