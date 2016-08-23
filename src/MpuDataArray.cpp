#include "MpuDataArray.h"
#include "defines.h"


float MpuDataArray::Get_omega_x() const {
	return _omega_x;
	}

float MpuDataArray::Get_omega_y() const {
	return _omega_y;
	}

float MpuDataArray::Get_omega_z() const {
	return _omega_z;
	}

float MpuDataArray::Get_Delta_V_x() const {
	return _Delta_V_x;
	}

float MpuDataArray::Get_Delta_V_y() const {
	return _Delta_V_y;
	}

float MpuDataArray::Get_Delta_V_z() const {
	return _Delta_V_z;
	}

float MpuDataArray::Get_Magnetometer_x_mgauss() const {
	return _magnetometer_x;
	}

float MpuDataArray::Get_Magnetometer_y_mgauss() const {
	return _magnetometer_y;
	}

float MpuDataArray::Get_Magnetometer_z_mgauss() const {
	return _magnetometer_z;
	}

float MpuDataArray::Get_Pressure_mbar() const {
	return _pressure;
	}

float MpuDataArray::Get_Temperature() const {
	return _temperature;
	}
	
void MpuDataArray::Set_omega_x(float a){
	_omega_x = a;
	} 

void MpuDataArray::Set_omega_y(float a){
	_omega_y = a;
	} 
	
void MpuDataArray::Set_omega_z(float a){
	_omega_z = a;
	} 
	
void MpuDataArray::Set_Delta_V_x(float a){
	_Delta_V_x = a;
	} 
	
void MpuDataArray::Set_Delta_V_y(float a){
	_Delta_V_y = a;
	} 
	
void MpuDataArray::Set_Delta_V_z(float a){
	_Delta_V_z = a;
	} 
	
void MpuDataArray::Set_Magnetometer_x_mgauss(float a){
	_magnetometer_x = a;
	}
	
void MpuDataArray::Set_Magnetometer_y_mgauss(float a){
	_magnetometer_y = a;
	}
	
void MpuDataArray::Set_Magnetometer_z_mgauss(float a){
	_magnetometer_z = a;
	}
	
void MpuDataArray::Set_Pressure_mbar(float a){
	_pressure = a;
	}
	
void MpuDataArray::Set_Temperature(float a){
	_temperature = a;
	} 
	
MpuDataArray::MpuDataArray()
	: _omega_x(0.0)
	, _omega_y(0.0)
	, _omega_z(0.0)
	, _Delta_V_x(0.0)
	, _Delta_V_y(0.0)
	, _Delta_V_z(0.0)
	, _magnetometer_x(0.0)
	, _magnetometer_y(0.0)
	, _magnetometer_z(0.0)
	, _pressure(0.0)
	, _temperature(0.0)
	{}

double MpuDataArrayWithTimestamp::GetTimestamp() const {
	return _timestamp;
	}

void MpuDataArrayWithTimestamp::SetTimestamp(double a) {
	_timestamp = a;
	}
	
MpuDataArrayWithTimestamp::MpuDataArrayWithTimestamp()
	: _timestamp(0.0)
	{}
