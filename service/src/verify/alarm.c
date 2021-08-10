#include "vm_config.h"
#include <stdint.h>
#include "mrubyc.h"
#include "thermostat.h"
#include <stdio.h>

char *OOOAlarmInfo = "OoO Alarm...\x00";
typedef struct alarm_info {
	char *ptr;
	uint8_t armed;
	int pin[4];
} alarm_info_t;

typedef struct alarm_ptr {
	alarm_info_t *alrm;
} alarm_ptr_t;

static void c_alarm_disarm(mrbc_vm *vm, mrbc_value v[], int argc) {
	alarm_ptr_t *apt = (alarm_ptr_t*) v->instance->data;
	alarm_info_t *ait = apt->alrm;
	if (ait->armed == 0) {
		SET_BOOL_RETURN(1);
		return;
	}
	if (argc != 4) {
		SET_BOOL_RETURN(0);
		return;
	}
	int p0 = GET_INT_ARG(1);
	int p1 = GET_INT_ARG(2);
	int p2 = GET_INT_ARG(3);
	int p3 = GET_INT_ARG(4);
	if (ait->pin[0] == p0 && ait->pin[1] == p1 && ait->pin[2] == p2 && ait->pin[3] == p3) {
		ait->armed = 0;
		SET_BOOL_RETURN(1);
		return;
	}
	SET_BOOL_RETURN(0);
	return;
}

static void c_alarm_arm(mrbc_vm *vm, mrbc_value v[], int argc) {
	alarm_ptr_t *apt = (alarm_ptr_t*) v->instance->data;
	apt->alrm->armed = 1;
}

static void c_alarm_armed(mrbc_vm *vm, mrbc_value v[], int argc) {
	alarm_ptr_t *apt = (alarm_ptr_t*) v->instance->data;
	uint8_t armed = apt->alrm->armed;
	SET_BOOL_RETURN(armed);
}

//================================================================
/*! info
  s = $alarm.info()
  @return String info
*/
static void c_alarm_info(mrbc_vm *vm, mrbc_value v[], int argc) {
	mrbc_value ret;
	alarm_ptr_t *apt = (alarm_ptr_t*) v->instance->data;
	ret = mrbc_string_new_cstr(vm, apt->alrm->ptr);
	SET_RETURN(ret);
}

//================================================================
/*! Alarm constructor
  $alarm = Alarm .new	# new alarm
*/
static void c_alarm_new(mrbc_vm *vm, mrbc_value v[], int argc) {
  *v = mrbc_instance_new(vm, v->cls, sizeof(alarm_ptr_t)); // calendar
  alarm_info_t *ait = mrbc_raw_alloc_no_free(sizeof(alarm_info_t));
  alarm_ptr_t *apt = (alarm_ptr_t*) v->instance->data;
  apt->alrm = ait;
  ait->ptr = OOOAlarmInfo;
  ait->armed = 0;
  ait->pin[0] = 1;
  ait->pin[1] = 3;
  ait->pin[2] = 3;
  ait->pin[3] = 7;
  return;
}

//================================================================
/*! initialize
*/
void mrbc_init_class_alarm(struct VM *vm) {
  // define class and methods.
  mrbc_class *alarm;
  alarm = mrbc_define_class(0, "Alarm", mrbc_class_object);
  mrbc_define_method(0, alarm , "new", c_alarm_new);
  mrbc_define_method(0, alarm, "info", c_alarm_info);
  mrbc_define_method(0, alarm, "armed?", c_alarm_armed);
  mrbc_define_method(0, alarm, "arm", c_alarm_arm);
  mrbc_define_method(0, alarm, "disarm", c_alarm_disarm);
}
