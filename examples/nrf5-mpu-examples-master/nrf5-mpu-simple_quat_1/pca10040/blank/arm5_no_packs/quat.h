
/*#include <math.h>



float pitch, yaw, roll;
//float deltat = 0.0f;                             // integration interval for both filter schemes
int lastUpdate = 0, firstUpdate = 0, Now = 0;    // used to calculate integration interval                               // used to calculate integration interval
float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};           // vector to hold quaternion
float eInt[3] = {0.0f, 0.0f, 0.0f};              // vector to hold integral error for Mahony method


const float PI = 3.14159265358979323846f;
const float GyroMeasError = PI*(60.0f / 180.0f);     // gyroscope measurement error in rads/s (start at 60 deg/s), then reduce after ~10 s to 3


float invSqrt(float x) { float halfx = 0.5f * x; float y = x;
long i = *(long*)&y;
i = 0x5f3759df - (i >> 1);
y = *(float*)&i;
y = y * (1.5f - (halfx * y * y)); return y;
}


void MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz,int deltat)
        {
					/*
						//float beta = sqrt(3.0f / 4.0f) * GyroMeasError;  // compute beta
					  float beta =0.1f;
						float GyroMeasDrift = PI * (1.0f / 180.0f);      // gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)
						float zeta = sqrt(3.0f / 4.0f) * GyroMeasDrift;  // compute zeta, the other free parameter in the Madgwick scheme usually set to a small or zero value
            //float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};   
            float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3];   // short name local variable for readability
            float norm;
            float hx, hy, _2bx, _2bz;
            float s1, s2, s3, s4;
            float qDot1, qDot2, qDot3, qDot4;

            // Auxiliary variables to avoid repeated arithmetic
            float _2q1mx;
            float _2q1my;
            float _2q1mz;
            float _2q2mx;
            float _4bx;
            float _4bz;
						float _8bx;
            float _8bz;
            float _2q1 = 2.0f * q1;
            float _2q2 = 2.0f * q2;
            float _2q3 = 2.0f * q3;
            float _2q4 = 2.0f * q4;
            float _2q1q3 = 2.0f * q1 * q3;
            float _2q3q4 = 2.0f * q3 * q4;
            float q1q1 = q1 * q1;
            float q1q2 = q1 * q2;
            float q1q3 = q1 * q3;
            float q1q4 = q1 * q4;
            float q2q2 = q2 * q2;
            float q2q3 = q2 * q3;
            float q2q4 = q2 * q4;
            float q3q3 = q3 * q3;
            float q3q4 = q3 * q4;
            float q4q4 = q4 * q4;

            // Normalise accelerometer measurement
            norm = sqrtf(ax * ax + ay * ay + az * az);
            if (norm == 0.0f) return; // handle NaN
            norm = 1.0f/norm;
            ax *= norm;
            ay *= norm;
            az *= norm;

            // Normalise magnetometer measurement
            norm = sqrtf(mx * mx + my * my + mz * mz);
            if (norm == 0.0f) return; // handle NaN
            norm = 1.0f/norm;
            mx *= norm;
            my *= norm;
            mz *= norm;

            // Reference direction of Earth's magnetic field
            _2q1mx = 2.0f * q1 * mx;
            _2q1my = 2.0f * q1 * my;
            _2q1mz = 2.0f * q1 * mz;
            _2q2mx = 2.0f * q2 * mx;
            hx = mx * q1q1 - _2q1my * q4 + _2q1mz * q3 + mx * q2q2 + _2q2 * my * q3 + _2q2 * mz * q4 - mx * q3q3 - mx * q4q4;
            hy = _2q1mx * q4 + my * q1q1 - _2q1mz * q2 + _2q2mx * q3 - my * q2q2 + my * q3q3 + _2q3 * mz * q4 - my * q4q4;
            _2bx = sqrt(hx * hx + hy * hy);
            _2bz = -_2q1mx * q3 + _2q1my * q2 + mz * q1q1 + _2q2mx * q4 - mz * q2q2 + _2q3 * my * q4 - mz * q3q3 + mz * q4q4;
            _4bx = 2.0f * _2bx;
            _4bz = 2.0f * _2bz;
						_8bx=2.0f * _4bx;
						_8bz=2.0f * _4bz;
            // Gradient decent algorithm corrective step
						/*
            s1 = -_2q3 * (2.0f * q2q4 - _2q1q3 - ax) + _2q2 * (2.0f * q1q2 + _2q3q4 - ay) - _2bz * q3 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q4 + _2bz * q2) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q3 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
            s2 = _2q4 * (2.0f * q2q4 - _2q1q3 - ax) + _2q1 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q2 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + _2bz * q4 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q3 + _2bz * q1) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q4 - _4bz * q2) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
            s3 = -_2q1 * (2.0f * q2q4 - _2q1q3 - ax) + _2q4 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q3 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + (-_4bx * q3 - _2bz * q1) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q2 + _2bz * q4) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q1 - _4bz * q3) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
            s4 = _2q2 * (2.0f * q2q4 - _2q1q3 - ax) + _2q3 * (2.0f * q1q2 + _2q3q4 - ay) + (-_4bx * q4 + _2bz * q2) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q1 + _2bz * q3) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q2 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
						*/
						/*s1= -_2q3*(2*(q2q4 - q1q3) - ax)    +   _2q2*(2*(q1q2 + q3q4) - ay)   +  -_4bz*q3*(_4bx*(0.5 - q3q3 - q4q4) + _4bz*(q2q4 - q1q3) - mx)   +   (-_4bx*q4+_4bz*q2)*(_4bx*(q2q3 - q1q4) + _4bz*(q1q2 + q3q4) - my)    +   _4bx*q3*(_4bx*(q1q3 + q2q4) + _4bz*(0.5 - q2q2 - q3q3) - mz);
						s2= _2q4*(2*(q2q4 - q1q3) - ax) +   _2q1*(2*(q1q2 + q3q4) - ay) +   -4*q2*(2*(0.5 - q2q2 - q3q3) - az)    +   _4bz*q4*(_4bx*(0.5 - q3q3 - q4q4) + _4bz*(q2q4 - q1q3) - mx)   + (_4bx*q3+_4bz*q1)*(_4bx*(q2q3 - q1q4) + _4bz*(q1q2 + q3q4) - my)   +   (_4bx*q4-_8bz*q2)*(_4bx*(q1q3 + q2q4) + _4bz*(0.5 - q2q2 - q3q3) - mz);             
						s3= -_2q1*(2*(q2q4 - q1q3) - ax)    +     _2q4*(2*(q1q2 + q3q4) - ay)   +   (-4*q3)*(2*(0.5 - q2q2 - q3q3) - az) +   (-_8bx*q3-_4bz*q1)*(_4bx*(0.5 - q3q3 - q4q4) + _4bz*(q2q4 - q1q3) - mx)+(_4bx*q2+_4bz*q4)*(_4bx*(q2q3 - q1q4) + _4bz*(q1q2 + q3q4) - my)+(_4bx*q1-_8bz*q3)*(_4bx*(q1q3 + q2q4) + _4bz*(0.5 - q2q2 - q3q3) - mz);
						s4= _2q2*(2*(q2q4 - q1q3) - ax) +   _2q3*(2*(q1q2 + q3q4) - ay)+(-_8bx*q4+_4bz*q2)*(_4bx*(0.5 - q3q3 - q4q4) + _4bz*(q2q4 - q1q3) - mx)+(-_4bx*q1+_4bz*q3)*(_4bx*(q2q3 - q1q4) + _4bz*(q1q2 + q3q4) - my)+(_4bx*q2)*(_4bx*(q1q3 + q2q4) + _4bz*(0.5 - q2q2 - q3q3) - mz);
            norm = sqrtf(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4);    // normalise step magnitude
            norm = 1.0f/norm;
            s1 *= norm;
            s2 *= norm;
            s3 *= norm;
            s4 *= norm;

            // Compute rate of change of quaternion
            qDot1 = 0.5f * (-q2 * gx - q3 * gy - q4 * gz) - beta * s1;
            qDot2 = 0.5f * (q1 * gx + q3 * gz - q4 * gy) - beta * s2;
            qDot3 = 0.5f * (q1 * gy - q2 * gz + q4 * gx) - beta * s3;
            qDot4 = 0.5f * (q1 * gz + q2 * gy - q3 * gx) - beta * s4;

            // Integrate to yield quaternion
            q1 += qDot1 * deltat;
            q2 += qDot2 * deltat;
            q3 += qDot3 * deltat;
            q4 += qDot4 * deltat;
            norm = sqrtf(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);    // normalise quaternion
            norm = 1.0f/norm;
            q[0] = q1 * norm;
            q[1] = q2 * norm;
            q[2] = q3 * norm;
            q[3] = q4 * norm;*/
				/*		float recipNorm;
						float beta =0.1f;
            float s0, s1, s2, s3;
            float qDot1, qDot2, qDot3, qDot4;
            float hx, hy;
						float q0 = q[0], q1 = q[1], q2 = q[2], q3 = q[3];
            float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1; float _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2; float q1q3, q2q2, q2q3, q3q3;
            qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz); qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy); qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx); qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);
						if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
							recipNorm = invSqrt(ax * ax + ay * ay + az * az);
							ax *= recipNorm;
              ay *= recipNorm;
              az *= recipNorm;
							recipNorm = invSqrt(mx * mx + my * my + mz * mz);
							mx *= recipNorm;
              my *= recipNorm;
              mz *= recipNorm;
							_2q0mx = 2.0f * q0 * mx;
              _2q0my = 2.0f * q0 * my;
              _2q0mz = 2.0f * q0 * mz;
              _2q1mx = 2.0f * q1 * mx; _2q0 = 2.0f * q0;
              _2q1 = 2.0f * q1;
              _2q2 = 2.0f * q2;
              _2q3 = 2.0f * q3;
             _2q0q2 = 2.0f * q0 * q2; _2q2q3 = 2.0f * q2 * q3; q0q0 = q0 * q0;
             q0q1 = q0 * q1;
						 q0q2 = q0 * q2; 
						 q0q3 = q0 * q3; 
						 q1q1 = q1 * q1; 
						 q1q2 = q1 * q2; 
						 q1q3 = q1 * q3; 
						 q2q2 = q2 * q2; 
						 q2q3 = q2 * q3; 
						 q3q3 = q3 * q3;
						 hx =mx*q0q0-_2q0my*q3+_2q0mz*q2+mx*q1q1+_2q1*my*q2+_2q1*mz*q3- mx*q2q2-mx*q3q3;
             hy =_2q0mx*q3+my*q0q0-_2q0mz*q1+_2q1mx*q2-my*q1q1+my*q2q2+ _2q2 *mz*q3-my*q3q3;
						 _2bx = sqrt(hx * hx + hy * hy);
             _2bz =-_2q0mx*q2+_2q0my*q1+mz*q0q0+_2q1mx*q3-mz*q1q1+_2q2*my*q3- mz*q2q2+mz*q3q3;
             _4bx = 2.0f * _2bx;
             _4bz = 2.0f * _2bz;
						 s0=-_2q2*(2.0f*q1q3-_2q0q2-ax)+_2q1*(2.0f*q0q1+_2q2q3-ay)- _2bz *q2*(_2bx*(0.5f-q2q2-q3q3)+_2bz*(q1q3-q0q2)-mx)+(-_2bx* q3+_2bz*q1)*(_2bx*(q1q2-q0q3)+_2bz*(q0q1+q2q3)-my)+_2bx*q2*(_2bx*(q0q2+q1q3)+_2bz*(0.5f-q1q1-q2q2)-mz);
             s1 =_2q3*(2.0f*q1q3-_2q0q2-ax)+_2q0*(2.0f*q0q1+_2q2q3-ay)- 4.0f *q1*(1-2.0f*q1q1-2.0f*q2q2-az)+_2bz*q3*(_2bx*(0.5f- q2q2 -q3q3)+_2bz*(q1q3-q0q2)-mx)+(_2bx*q2+_2bz*q0)*(_2bx* (q1q2 -q0q3)+_2bz*(q0q1+q2q3)-my)+(_2bx*q3-_4bz*q1)*(_2bx*(q0q2 +q1q3)+_2bz*(0.5f-q1q1-q2q2)-mz);
             s2 =-_2q0*(2.0f*q1q3-_2q0q2-ax)+_2q3*(2.0f*q0q1+_2q2q3-ay)- 4.0f *q2*(1-2.0f*q1q1-2.0f*q2q2-az)+(-_4bx*q2-_2bz*q0)* (_2bx*(0.5f-q2q2-q3q3)+_2bz*(q1q3-q0q2)-mx)+(_2bx*q1+_2bz* q3)*(_2bx*(q1q2-q0q3)+_2bz*(q0q1+q2q3)-my)+(_2bx*q0-_4bz* q2) *(_2bx*(q0q2+q1q3)+_2bz*(0.5f-q1q1-q2q2)-mz);
             s3 =_2q1*(2.0f*q1q3-_2q0q2-ax)+_2q2*(2.0f*q0q1+_2q2q3-ay)+ (-_4bx*q3+_2bz*q1)*(_2bx*(0.5f-q2q2-q3q3)+_2bz*(q1q3-q0q2)- mx)+(-_2bx*q0+_2bz*q2)*(_2bx*(q1q2-q0q3)+_2bz*(q0q1+q2q3)- my) +_2bx*q1*(_2bx*(q0q2+q1q3)+_2bz*(0.5f-q1q1-q2q2)-mz);
             recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3);
						 s0 *= recipNorm;
             s1 *= recipNorm;
             s2 *= recipNorm; s3 *= recipNorm;
						 qDot1 -= beta * s0; qDot2 -= beta * s1; qDot3 -= beta * s2; qDot4 -= beta * s3;
             }
						q0 += qDot1 * deltat;
            q1 += qDot2 * deltat;
            q2 += qDot3 * deltat;
            q3 += qDot4 * deltat;
						recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
						q0 *= recipNorm;
            q1 *= recipNorm;
            q2 *= recipNorm;
            q3 *= recipNorm;
						 q[0]=q0;
						 q[1]=q1;
						 q[2]=q2;
						 q[3]=q3;

        }*/
				
				#include <math.h>



float pitch, yaw, roll;
//float deltat = 0.0f;                             // integration interval for both filter schemes
int lastUpdate = 0, firstUpdate = 0, Now = 0;    // used to calculate integration interval                               // used to calculate integration interval
float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};           // vector to hold quaternion
float eInt[3] = {0.0f, 0.0f, 0.0f};              // vector to hold integral error for Mahony method


const float PI = 3.14159265358979323846f;
const float GyroMeasError = PI*(60.0f / 180.0f);     // gyroscope measurement error in rads/s (start at 60 deg/s), then reduce after ~10 s to 3





void MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz,float deltat)
        {
						//float beta = sqrt(3.0f / 4.0f) * GyroMeasError;  // compute beta
						float beta =0.1f;
						float GyroMeasDrift = PI * (1.0f / 180.0f);      // gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)
						float zeta = sqrt(3.0f / 4.0f) * GyroMeasDrift;  // compute zeta, the other free parameter in the Madgwick scheme usually set to a small or zero value

            float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3];   // short name local variable for readability
            float norm;
            float hx, hy, _2bx, _2bz;
            float s1, s2, s3, s4;
            float qDot1, qDot2, qDot3, qDot4;

            // Auxiliary variables to avoid repeated arithmetic
            float _2q1mx;
            float _2q1my;
            float _2q1mz;
            float _2q2mx;
            float _4bx;
            float _4bz;
						float _8bx;
            float _8bz;
            float _2q1 = 2.0f * q1;
            float _2q2 = 2.0f * q2;
            float _2q3 = 2.0f * q3;
            float _2q4 = 2.0f * q4;
            float _2q1q3 = 2.0f * q1 * q3;
            float _2q3q4 = 2.0f * q3 * q4;
            float q1q1 = q1 * q1;
            float q1q2 = q1 * q2;
            float q1q3 = q1 * q3;
            float q1q4 = q1 * q4;
            float q2q2 = q2 * q2;
            float q2q3 = q2 * q3;
            float q2q4 = q2 * q4;
            float q3q3 = q3 * q3;
            float q3q4 = q3 * q4;
            float q4q4 = q4 * q4;

            // Normalise accelerometer measurement
            norm = sqrtf(ax * ax + ay * ay + az * az);
            if (norm == 0.0f) return; // handle NaN
            norm = 1.0f/norm;
            ax *= norm;
            ay *= norm;
            az *= norm;

            // Normalise magnetometer measurement
            norm = sqrtf(mx * mx + my * my + mz * mz);
            if (norm == 0.0f) return; // handle NaN
            norm = 1.0f/norm;
            mx *= norm;
            my *= norm;
            mz *= norm;

            // Reference direction of Earth's magnetic field
            _2q1mx = 2.0f * q1 * mx;
            _2q1my = 2.0f * q1 * my;
            _2q1mz = 2.0f * q1 * mz;
            _2q2mx = 2.0f * q2 * mx;
            hx = mx * q1q1 - _2q1my * q4 + _2q1mz * q3 + mx * q2q2 + _2q2 * my * q3 + _2q2 * mz * q4 - mx * q3q3 - mx * q4q4;
            hy = _2q1mx * q4 + my * q1q1 - _2q1mz * q2 + _2q2mx * q3 - my * q2q2 + my * q3q3 + _2q3 * mz * q4 - my * q4q4;
            _2bx = sqrt(hx * hx + hy * hy);
            _2bz = -_2q1mx * q3 + _2q1my * q2 + mz * q1q1 + _2q2mx * q4 - mz * q2q2 + _2q3 * my * q4 - mz * q3q3 + mz * q4q4;
            _4bx = 2.0f * _2bx;
            _4bz = 2.0f * _2bz;
						_8bx=2.0f * _4bx;
						_8bz=2.0f * _4bz;
            // Gradient decent algorithm corrective step
						
            s1 = -_2q3 * (2.0f * q2q4 - _2q1q3 - ax) + _2q2 * (2.0f * q1q2 + _2q3q4 - ay) - _2bz * q3 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q4 + _2bz * q2) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q3 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
            s2 = _2q4 * (2.0f * q2q4 - _2q1q3 - ax) + _2q1 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q2 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + _2bz * q4 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q3 + _2bz * q1) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q4 - _4bz * q2) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
            s3 = -_2q1 * (2.0f * q2q4 - _2q1q3 - ax) + _2q4 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q3 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + (-_4bx * q3 - _2bz * q1) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q2 + _2bz * q4) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q1 - _4bz * q3) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
            s4 = _2q2 * (2.0f * q2q4 - _2q1q3 - ax) + _2q3 * (2.0f * q1q2 + _2q3q4 - ay) + (-_4bx * q4 + _2bz * q2) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q1 + _2bz * q3) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q2 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
						/*
						s1= -_2q3*(2*(q2q4 - q1q3) - ax)    +   _2q2*(2*(q1q2 + q3q4) - ay)   +  -_4bz*q3*(_4bx*(0.5 - q3q3 - q4q4) + _4bz*(q2q4 - q1q3) - mx)   +   (-_4bx*q4+_4bz*q2)*(_4bx*(q2q3 - q1q4) + _4bz*(q1q2 + q3q4) - my)    +   _4bx*q3*(_4bx*(q1q3 + q2q4) + _4bz*(0.5 - q2q2 - q3q3) - mz);
						s2= _2q4*(2*(q2q4 - q1q3) - ax) +   _2q1*(2*(q1q2 + q3q4) - ay) +   -4*q2*(2*(0.5 - q2q2 - q3q3) - az)    +   _4bz*q4*(_4bx*(0.5 - q3q3 - q4q4) + _4bz*(q2q4 - q1q3) - mx)   + (_4bx*q3+_4bz*q1)*(_4bx*(q2q3 - q1q4) + _4bz*(q1q2 + q3q4) - my)   +   (_4bx*q4-_8bz*q2)*(_4bx*(q1q3 + q2q4) + _4bz*(0.5 - q2q2 - q3q3) - mz);             
						s3= -_2q1*(2*(q2q4 - q1q3) - ax)    +     _2q4*(2*(q1q2 + q3q4) - ay)   +   (-4*q3)*(2*(0.5 - q2q2 - q3q3) - az) +   (-_8bx*q3-_4bz*q1)*(_4bx*(0.5 - q3q3 - q4q4) + _4bz*(q2q4 - q1q3) - mx)+(_4bx*q2+_4bz*q4)*(_4bx*(q2q3 - q1q4) + _4bz*(q1q2 + q3q4) - my)+(_4bx*q1-_8bz*q3)*(_4bx*(q1q3 + q2q4) + _4bz*(0.5 - q2q2 - q3q3) - mz);
						s4= _2q2*(2*(q2q4 - q1q3) - ax) +   _2q3*(2*(q1q2 + q3q4) - ay)+(-_8bx*q4+_4bz*q2)*(_4bx*(0.5 - q3q3 - q4q4) + _4bz*(q2q4 - q1q3) - mx)+(-_4bx*q1+_4bz*q3)*(_4bx*(q2q3 - q1q4) + _4bz*(q1q2 + q3q4) - my)+(_4bx*q2)*(_4bx*(q1q3 + q2q4) + _4bz*(0.5 - q2q2 - q3q3) - mz);
						*/
            norm = sqrtf(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4);    // normalise step magnitude
            norm = 1.0f/norm;
            s1 *= norm;
            s2 *= norm;
            s3 *= norm;
            s4 *= norm;

            // Compute rate of change of quaternion
            qDot1 = 0.5f * (-q2 * gx - q3 * gy - q4 * gz) - beta * s1;
            qDot2 = 0.5f * (q1 * gx + q3 * gz - q4 * gy) - beta * s2;
            qDot3 = 0.5f * (q1 * gy - q2 * gz + q4 * gx) - beta * s3;
            qDot4 = 0.5f * (q1 * gz + q2 * gy - q3 * gx) - beta * s4;

            // Integrate to yield quaternion
            q1 += qDot1 * deltat;
            q2 += qDot2 * deltat;
            q3 += qDot3 * deltat;
            q4 += qDot4 * deltat;
            norm = sqrtf(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);    // normalise quaternion
            norm = 1.0f/norm;
            q[0] = q1 * norm;
            q[1] = q2 * norm;
            q[2] = q3 * norm;
            q[3] = q4 * norm;

        }