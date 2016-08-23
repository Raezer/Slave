#ifndef MPUDATAARRAY_H
#define MPUDATAARRAY_H

#include <stdint.h>
#include "defines.h"

#pragma pack(push, 1)

class MpuDataArray
	{
private:
	float	_omega_x;
 	float	_omega_y;
 	float	_omega_z;
 	float	_Delta_V_x;
 	float	_Delta_V_y;
 	float	_Delta_V_z;
	float 	_magnetometer_x;
	float 	_magnetometer_y;
	float 	_magnetometer_z;
	float 	_pressure;
	float	_temperature;

	float _AccelCode2Float(int32_t a) const;
	float _OmegaCode2Float(int32_t a) const;
	float _MagnetometerCode2Float(int16_t a) const;

public:
	MpuDataArray();
	float Get_omega_x() const; 
	float Get_omega_y() const; 
	float Get_omega_z() const; 
	float Get_Delta_V_x() const; 
	float Get_Delta_V_y() const; 
	float Get_Delta_V_z() const; 
	float Get_Magnetometer_x_mgauss() const;
	float Get_Magnetometer_y_mgauss() const;
	float Get_Magnetometer_z_mgauss() const;
	float Get_Pressure_mbar() const;
	float Get_Temperature() const; 
	void Set_omega_x(float a); 
	void Set_omega_y(float a); 
	void Set_omega_z(float a); 
	void Set_Delta_V_x(float a); 
	void Set_Delta_V_y(float a); 
	void Set_Delta_V_z(float a); 
	void Set_Magnetometer_x_mgauss(float a);
	void Set_Magnetometer_y_mgauss(float a);
	void Set_Magnetometer_z_mgauss(float a);
	void Set_Pressure_mbar(float a);
	void Set_Temperature(float a); 
	};

class MpuDataArrayWithTimestamp
	: public MpuDataArray
	{
	double _timestamp;
public:
	MpuDataArrayWithTimestamp();
	double GetTimestamp() const;
	void SetTimestamp(double a);
	};

#pragma pack(pop)

#endif // MPUDATAARRAY_H
