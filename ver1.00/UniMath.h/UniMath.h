/**
*	@file	UniMath.h
*	@brief
*	@author	MASAKAZU KOTAKE
*	@date	UPDATE : 2014/12/13
*/
#ifndef _UNI_MATH_H_
#define _UNI_MATH_H_

#include <list>
#include <iostream>

namespace UniMath{

	//�O���錾
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix3;
	class Matrix4;
	class Ray;
	class Math;


	class Radian
	{
		float _rad;

	public:
		explicit Radian ( float r=0 );
		Radian ( const Degree& d );
		Radian& operator = ( const float& f );
		Radian& operator = ( const Radian& r );
		Radian& operator = ( const Degree& d );

		float valueDegrees() const; // see bottom of this file
		float valueRadians() const;
		float valueAngleUnits() const;

		const Radian& operator + () const;
		Radian operator + ( const Radian& r ) const;
		Radian operator + ( const Degree& d ) const;
		Radian& operator += ( const Radian& r );
		Radian& operator += ( const Degree& d );
		Radian operator - () const;
		Radian operator - ( const Radian& r ) const;
		Radian operator - ( const Degree& d ) const;
		Radian& operator -= ( const Radian& r );
		Radian& operator -= ( const Degree& d );
		Radian operator * ( float f ) const;
        Radian operator * ( const Radian& f ) const;
		Radian& operator *= ( float f );
		Radian operator / ( float f ) const;
		Radian& operator /= ( float f );

		bool operator <  ( const Radian& r ) const;
		bool operator <= ( const Radian& r ) const;
		bool operator == ( const Radian& r ) const;
		bool operator != ( const Radian& r ) const;
		bool operator >= ( const Radian& r ) const;
		bool operator >  ( const Radian& r ) const;

		inline friend std::ostream& operator <<
			( std::ostream& o, const Radian& v );
	};

	/** Wrapper class which indicates a given angle value is in Degrees.
	@remarks
		Degree values are interchangeable with Radian values, and conversions
		will be done automatically between them.
	*/
	class Degree
	{
		float _deg; // if you get an error here - make sure to define/typedef 'float' first

	public:
		explicit Degree ( float d=0 ) : mDeg(d) {}
		Degree ( const Radian& r ) : mDeg(r.valueDegrees()) {}
		Degree& operator = ( const float& f ) { mDeg = f; return *this; }
		Degree& operator = ( const Degree& d ) { mDeg = d.mDeg; return *this; }
		Degree& operator = ( const Radian& r ) { mDeg = r.valueDegrees(); return *this; }

		float valueDegrees() const { return mDeg; }
		float valueRadians() const; // see bottom of this file
		float valueAngleUnits() const;

		const Degree& operator + () const { return *this; }
		Degree operator + ( const Degree& d ) const { return Degree ( mDeg + d.mDeg ); }
		Degree operator + ( const Radian& r ) const { return Degree ( mDeg + r.valueDegrees() ); }
		Degree& operator += ( const Degree& d ) { mDeg += d.mDeg; return *this; }
		Degree& operator += ( const Radian& r ) { mDeg += r.valueDegrees(); return *this; }
		Degree operator - () const { return Degree(-mDeg); }
		Degree operator - ( const Degree& d ) const { return Degree ( mDeg - d.mDeg ); }
		Degree operator - ( const Radian& r ) const { return Degree ( mDeg - r.valueDegrees() ); }
		Degree& operator -= ( const Degree& d ) { mDeg -= d.mDeg; return *this; }
		Degree& operator -= ( const Radian& r ) { mDeg -= r.valueDegrees(); return *this; }
		Degree operator * ( float f ) const { return Degree ( mDeg * f ); }
        Degree operator * ( const Degree& f ) const { return Degree ( mDeg * f.mDeg ); }
		Degree& operator *= ( float f ) { mDeg *= f; return *this; }
		Degree operator / ( float f ) const { return Degree ( mDeg / f ); }
		Degree& operator /= ( float f ) { mDeg /= f; return *this; }

		bool operator <  ( const Degree& d ) const { return mDeg <  d.mDeg; }
		bool operator <= ( const Degree& d ) const { return mDeg <= d.mDeg; }
		bool operator == ( const Degree& d ) const { return mDeg == d.mDeg; }
		bool operator != ( const Degree& d ) const { return mDeg != d.mDeg; }
		bool operator >= ( const Degree& d ) const { return mDeg >= d.mDeg; }
		bool operator >  ( const Degree& d ) const { return mDeg >  d.mDeg; }

		inline friend std::ostream& operator <<
			( std::ostream& o, const Degree& v )
		{
			o << "Degree(" << v.valueDegrees() << ")";
			return o;
		}
	};

	/** Wrapper class which identifies a value as the currently default angle 
		type, as defined by Math::setAngleUnit.
	@remarks
		Angle values will be automatically converted between radians and degrees,
		as appropriate.
	*/
	class Angle
	{
		float mAngle;
	public:
		explicit Angle ( float angle ) : mAngle(angle) {}
		operator Radian() const;
		operator Degree() const;
	};

	// these functions could not be defined within the class definition of class
	// Radian because they required class Degree to be defined
	inline Radian::Radian ( const Degree& d ) : mRad(d.valueRadians()) {
	}
	inline Radian& Radian::operator = ( const Degree& d ) {
		mRad = d.valueRadians(); return *this;
	}
	inline Radian Radian::operator + ( const Degree& d ) const {
		return Radian ( mRad + d.valueRadians() );
	}
	inline Radian& Radian::operator += ( const Degree& d ) {
		mRad += d.valueRadians();
		return *this;
	}
	inline Radian Radian::operator - ( const Degree& d ) const {
		return Radian ( mRad - d.valueRadians() );
	}
	inline Radian& Radian::operator -= ( const Degree& d ) {
		mRad -= d.valueRadians();
		return *this;
	}

	/** Class to provide access to common mathematical functions.
		@remarks
			Most of the maths functions are aliased versions of the C runtime
			library functions. They are aliased here to provide future
			optimisation opportunities, either from faster RTLs or custom
			math approximations.
		@note
			<br>This is based on MgcMath.h from
			<a href="http://www.geometrictools.com/">Wild Magic</a>.
	*/
	class Math 
	{
	public:
		/** The angular units used by the API. This functionality is now deprecated in favor
			of discreet angular unit types ( see Degree and Radian above ). The only place
			this functionality is actually still used is when parsing files. Search for
			usage of the Angle class for those instances
		*/
		enum AngleUnit
		{
			AU_DEGREE,
			AU_RADIAN
		};


		/** This class is used to provide an external random value provider. 
		*/
		class RandomValueProvider
		{
		public:
			virtual ~RandomValueProvider() {}
			
			/** When called should return a random values in the range of [0,1] */
			virtual float getRandomUnit() = 0;
	};

	protected:
		/// Angle units used by the api
		static AngleUnit msAngleUnit;

		/// Size of the trig tables as determined by constructor.
		static int mTrigTableSize;

		/// Radian -> index factor value ( mTrigTableSize / 2 * PI )
		static float mTrigTableFactor;
		static float* mSinTable;
		static float* mTanTable;

		/// A random value provider. overriding the default random number generator.
		static RandomValueProvider* mRandProvider;

		/** Private function to build trig tables.
		*/
		void buildTrigTables();

		static float SinTable (float fValue);
		static float TanTable (float fValue);
	public:
		/** Default constructor.
			@param
				trigTableSize Optional parameter to set the size of the
				tables used to implement Sin, Cos, Tan
		*/
		Math(unsigned int trigTableSize = 4096);

		/** Default destructor.
		*/
		~Math();

		static inline int IAbs (int iValue) { return ( iValue >= 0 ? iValue : -iValue ); }
		static inline int ICeil (float fValue) { return int(ceil(fValue)); }
		static inline int IFloor (float fValue) { return int(floor(fValue)); }
		static int ISign (int iValue);

		/** Absolute value function
			@param
				fValue The value whose absolute value will be returned.
		*/
		static inline float Abs (float fValue) { return float(fabs(fValue)); }

		/** Absolute value function
			@param dValue
				The value, in degrees, whose absolute value will be returned.
		 */
		static inline Degree Abs (const Degree& dValue) { return Degree(fabs(dValue.valueDegrees())); }

		/** Absolute value function
			@param rValue
				The value, in radians, whose absolute value will be returned.
		*/
		static inline Radian Abs (const Radian& rValue) { return Radian(fabs(rValue.valueRadians())); }

		/** Arc cosine function
			@param fValue
				The value whose arc cosine will be returned.
		*/
		static Radian ACos (float fValue);

		/** Arc sine function
			@param fValue
				The value whose arc sine will be returned.
		*/
		static Radian ASin (float fValue);

		/** Arc tangent function
			@param fValue
				The value whose arc tangent will be returned.
		*/
		static inline Radian ATan (float fValue) { return Radian(atan(fValue)); }

		/** Arc tangent between two values function
			@param fY
				The first value to calculate the arc tangent with.
			@param fX
				The second value to calculate the arc tangent with.
		*/
		static inline Radian ATan2 (float fY, float fX) { return Radian(atan2(fY,fX)); }

		/** Ceiling function
			Returns the smallest following integer. (example: Ceil(1.1) = 2)

			@param fValue
				The value to round up to the nearest integer.
		*/
		static inline float Ceil (float fValue) { return float(ceil(fValue)); }
		static inline bool isNaN(float f)
		{
			// std::isnan() is C99, not supported by all compilers
			// However NaN always fails this next test, no other number does.
			return f != f;
		}

		/** Cosine function.
			@param fValue
				Angle in radians
			@param useTables
				If true, uses lookup tables rather than
				calculation - faster but less accurate.
		*/
		static inline float Cos (const Radian& fValue, bool useTables = false) {
			return (!useTables) ? float(cos(fValue.valueRadians())) : SinTable(fValue.valueRadians() + HALF_PI);
		}

		/** Cosine function.
			@param fValue
				Angle in radians
			@param useTables
				If true, uses lookup tables rather than
				calculation - faster but less accurate.
		*/
		static inline float Cos (float fValue, bool useTables = false) {
			return (!useTables) ? float(cos(fValue)) : SinTable(fValue + HALF_PI);
		}

		static inline float Exp (float fValue) { return float(exp(fValue)); }

		/** Floor function
		    Returns the largest previous integer. (example: Floor(1.9) = 1)
		 
		    @param fValue
		        The value to round down to the nearest integer.
		 */
		static inline float Floor (float fValue) { return float(floor(fValue)); }

		static inline float Log (float fValue) { return float(log(fValue)); }

		/// Stored value of log(2) for frequent use
		static const float LOG2;

		static inline float Log2 (float fValue) { return float(log(fValue)/LOG2); }

		static inline float LogN (float base, float fValue) { return float(log(fValue)/log(base)); }

		static inline float Pow (float fBase, float fExponent) { return float(pow(fBase,fExponent)); }

		static float Sign (float fValue);
		static inline Radian Sign ( const Radian& rValue )
		{
		    return Radian(Sign(rValue.valueRadians()));
		}
		static inline Degree Sign ( const Degree& dValue )
		{
		    return Degree(Sign(dValue.valueDegrees()));
		}

		//Simulate the shader function saturate that clamps a parameter value between 0 and 1
		static inline float saturate(float t) { return (t < 0) ? 0 : ((t > 1) ? 1 : t); }
		static inline double saturate(double t) { return (t < 0) ? 0 : ((t > 1) ? 1 : t); }
		
		//Simulate the shader function lerp which performers linear interpolation
		//given 3 parameters v0, v1 and t the function returns the value of (1 �Et)* v0 + t * v1. 
		//where v0 and v1 are matching vector or scalar types and t can be either a scalar or a vector of the same type as a and b.
		template<typename V, typename T> static V lerp(const V& v0, const V& v1, const T& t) { 
		    return v0 * (1 - t) + v1 * t; }

		/** Sine function.
			@param fValue
				Angle in radians
			@param useTables
				If true, uses lookup tables rather than
				calculation - faster but less accurate.
		*/
		static inline float Sin (const Radian& fValue, bool useTables = false) {
			return (!useTables) ? float(sin(fValue.valueRadians())) : SinTable(fValue.valueRadians());
		}

		/** Sine function.
			@param fValue
				Angle in radians
			@param useTables
				If true, uses lookup tables rather than
				calculation - faster but less accurate.
		*/
		static inline float Sin (float fValue, bool useTables = false) {
			return (!useTables) ? float(sin(fValue)) : SinTable(fValue);
		}

		/** Squared function.
			@param fValue
				The value to be squared (fValue^2)
		*/
		static inline float Sqr (float fValue) { return fValue*fValue; }

		/** Square root function.
			@param fValue
				The value whose square root will be calculated.
		 */
		static inline float Sqrt (float fValue) { return float(sqrt(fValue)); }

		/** Square root function.
			@param fValue
				The value, in radians, whose square root will be calculated.
		    @return
				The square root of the angle in radians.
		 */
		static inline Radian Sqrt (const Radian& fValue) { return Radian(sqrt(fValue.valueRadians())); }

		/** Square root function.
			@param fValue
				The value, in degrees, whose square root will be calculated.
			@return
				The square root of the angle in degrees.
		 */
		static inline Degree Sqrt (const Degree& fValue) { return Degree(sqrt(fValue.valueDegrees())); }

		/** Inverse square root i.e. 1 / Sqrt(x), good for vector
			normalisation.
			@param fValue
				The value whose inverse square root will be calculated.
		*/
		static float InvSqrt (float fValue);

		/** Generate a random number of unit length.
			@return
				A random number in the range from [0,1].
		*/
		static float UnitRandom ();

		/** Generate a random number within the range provided.
			@param fLow
				The lower bound of the range.
			@param fHigh
				The upper bound of the range.
			@return
				A random number in the range from [fLow,fHigh].
		 */
		static float RangeRandom (float fLow, float fHigh);

		/** Generate a random number in the range [-1,1].
			@return
				A random number in the range from [-1,1].
		 */
		static float SymmetricRandom ();

		static void SetRandomValueProvider(RandomValueProvider* provider);

		/** Tangent function.
			@param fValue
				Angle in radians
			@param useTables
				If true, uses lookup tables rather than
				calculation - faster but less accurate.
		*/
		static inline float Tan (const Radian& fValue, bool useTables = false) {
			return (!useTables) ? float(tan(fValue.valueRadians())) : TanTable(fValue.valueRadians());
		}

		/** Tangent function.
			@param fValue
				Angle in radians
			@param useTables
				If true, uses lookup tables rather than
				calculation - faster but less accurate.
		*/
		static inline float Tan (float fValue, bool useTables = false) {
			return (!useTables) ? float(tan(fValue)) : TanTable(fValue);
		}

		static inline float DegreesToRadians(float degrees) { return degrees * fDeg2Rad; }
		static inline float RadiansToDegrees(float radians) { return radians * fRad2Deg; }

		/** These functions used to set the assumed angle units (radians or degrees) 
			expected when using the Angle type.
		@par
			You can set this directly after creating a new Root, and also before/after resource creation,
			depending on whether you want the change to affect resource files.
		*/
		static void setAngleUnit(AngleUnit unit);
		
		/** Get the unit being used for angles. */
		static AngleUnit getAngleUnit(void);

		/** Convert from the current AngleUnit to radians. */
		static float AngleUnitsToRadians(float units);
		/** Convert from radians to the current AngleUnit . */
		static float RadiansToAngleUnits(float radians);
		/** Convert from the current AngleUnit to degrees. */
		static float AngleUnitsToDegrees(float units);
		/** Convert from degrees to the current AngleUnit. */
		static float DegreesToAngleUnits(float degrees);

		/** Checks whether a given point is inside a triangle, in a
			2-dimensional (Cartesian) space.
			@remarks
				The vertices of the triangle must be given in either
				trigonometrical (anticlockwise) or inverse trigonometrical
				(clockwise) order.
			@param p
				The point.
			@param a
				The triangle's first vertex.
			@param b
				The triangle's second vertex.
			@param c
				The triangle's third vertex.
			@return
				If the point resides in the triangle, <b>true</b> is
				returned.
			@par
				If the point is outside the triangle, <b>false</b> is
				returned.
		*/
		static bool pointInTri2D(const Vector2& p, const Vector2& a, 
			const Vector2& b, const Vector2& c);

		/** Checks whether a given 3D point is inside a triangle.
		@remarks
			The vertices of the triangle must be given in either
			trigonometrical (anticlockwise) or inverse trigonometrical
			(clockwise) order, and the point must be guaranteed to be in the
			same plane as the triangle
		@param p
			p The point.
		@param a
			The triangle's first vertex.
		@param b
			The triangle's second vertex.
		@param c
			The triangle's third vertex.
		@param normal
			The triangle plane's normal (passed in rather than calculated
			on demand since the caller may already have it)
		@return
			If the point resides in the triangle, <b>true</b> is
			returned.
		@par
			If the point is outside the triangle, <b>false</b> is
			returned.
		*/
		static bool pointInTri3D(const Vector3& p, const Vector3& a, 
			const Vector3& b, const Vector3& c, const Vector3& normal);
		/** Ray / plane intersection, returns boolean result and distance. */
		static std::pair<bool, float> intersects(const Ray& ray, const Plane& plane);

		/** Ray / sphere intersection, returns boolean result and distance. */
		static std::pair<bool, float> intersects(const Ray& ray, const Sphere& sphere, 
			bool discardInside = true);
		
		/**	Ray / box intersection, returns boolean result and distance. */
		static std::pair<bool, float> intersects(const Ray& ray, const AxisAlignedBox& box);

		/**	Ray / box intersection, returns boolean result and two intersection distance.
		@param ray
			The ray.
		@param box
			The box.
		@param d1
			A float pointer to retrieve the near intersection distance
			from the ray origin, maybe <b>null</b> which means don't care
			about the near intersection distance.
		@param d2
			A float pointer to retrieve the far intersection distance
			from the ray origin, maybe <b>null</b> which means don't care
			about the far intersection distance.
		@return
			If the ray is intersects the box, <b>true</b> is returned, and
			the near intersection distance is return by <i>d1</i>, the
			far intersection distance is return by <i>d2</i>. Guarantee
			<b>0</b> <= <i>d1</i> <= <i>d2</i>.
		@par
			If the ray isn't intersects the box, <b>false</b> is returned, and
			<i>d1</i> and <i>d2</i> is unmodified.
		*/
		static bool intersects(const Ray& ray, const AxisAlignedBox& box,
			float* d1, float* d2);

		/**	Ray / triangle intersection, returns boolean result and distance.
		@param ray
			The ray.
		@param a
			The triangle's first vertex.
		@param b
			The triangle's second vertex.
		@param c
			The triangle's third vertex.
		@param normal
			The triangle plane's normal (passed in rather than calculated
			on demand since the caller may already have it), doesn't need
			normalised since we don't care.
		@param positiveSide
			Intersect with "positive side" of the triangle
		@param negativeSide
			Intersect with "negative side" of the triangle
		@return
			If the ray is intersects the triangle, a pair of <b>true</b> and the
			distance between intersection point and ray origin returned.
		@par
			If the ray isn't intersects the triangle, a pair of <b>false</b> and
			<b>0</b> returned.
		*/
		static std::pair<bool, float> intersects(const Ray& ray, const Vector3& a,
			const Vector3& b, const Vector3& c, const Vector3& normal,
			bool positiveSide = true, bool negativeSide = true);

		/**	Ray / triangle intersection, returns boolean result and distance.
		@param ray
			The ray.
		@param a
			The triangle's first vertex.
		@param b
			The triangle's second vertex.
		@param c
			The triangle's third vertex.
		@param positiveSide
			Intersect with "positive side" of the triangle
		@param negativeSide
			Intersect with "negative side" of the triangle
		@return
			If the ray is intersects the triangle, a pair of <b>true</b> and the
			distance between intersection point and ray origin returned.
		@par
			If the ray isn't intersects the triangle, a pair of <b>false</b> and
			<b>0</b> returned.
		*/
		static std::pair<bool, float> intersects(const Ray& ray, const Vector3& a,
			const Vector3& b, const Vector3& c,
			bool positiveSide = true, bool negativeSide = true);

		/**	Sphere / box intersection test. */
		static bool intersects(const Sphere& sphere, const AxisAlignedBox& box);

		/**	Plane / box intersection test. */
		static bool intersects(const Plane& plane, const AxisAlignedBox& box);

		/**	Ray / convex plane list intersection test. 
		@param ray The ray to test with
		@param planeList List of planes which form a convex volume
		@param normalIsOutside Does the normal point outside the volume
		*/
		static std::pair<bool, float> intersects( const Ray& ray, const vector<Plane>& planeList,  bool normalIsOutside);
		
		/**	Ray / convex plane list intersection test. 
		@param ray The ray to test with
		@param planeList List of planes which form a convex volume
		@param normalIsOutside Does the normal point outside the volume
		*/
		static std::pair<bool, float> intersects(
			const Ray& ray, const list<Plane>& planeList, 
			bool normalIsOutside);

		/**	Sphere / plane intersection test. 
		@remarks NB just do a plane.getDistance(sphere.getCenter()) for more detail!
		*/
		static bool intersects(const Sphere& sphere, const Plane& plane);

		/**	Compare 2 floats, using tolerance for inaccuracies.
		*/
		static bool floatEqual(float a, float b,
			float tolerance = std::numeric_limits<float>::epsilon());

		/**	Calculates the tangent space vector for a given set of positions / texture coords. */
		static Vector3 calculateTangentSpaceVector(
			const Vector3& position1, const Vector3& position2, const Vector3& position3,
			float u1, float v1, float u2, float v2, float u3, float v3);

		/**	Build a reflection matrix for the passed in plane. */
		static Matrix4 buildReflectionMatrix(const Plane& p);
		/**	Calculate a face normal, including the w component which is the offset from the origin. */
		static Vector4 calculateFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
		/**	Calculate a face normal, no w-information. */
		static Vector3 calculateBasicFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
		/**	Calculate a face normal without normalize, including the w component which is the offset from the origin. */
		static Vector4 calculateFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);
		/**	Calculate a face normal without normalize, no w-information. */
		static Vector3 calculateBasicFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);

		/** Generates a value based on the Gaussian (normal) distribution function
			with the given offset and scale parameters.
		*/
		static float gaussianDistribution(float x, float offset = 0.0f, float scale = 1.0f);

		/** Clamp a value within an inclusive range. */
		template <typename T>
		static T Clamp(T val, T minval, T maxval)
		{
			assert (minval <= maxval && "Invalid clamp range");
			return std::max(std::min(val, maxval), minval);
		}

		static Matrix4 makeViewMatrix(const Vector3& position, const Quaternion& orientation, 
			const Matrix4* reflectMatrix = 0);

		/** Get a bounding radius value from a bounding box. */
		static float boundingRadiusFromAABB(const AxisAlignedBox& aabb);



		static const float POS_INFINITY;
		static const float NEG_INFINITY;
		static const float PI;
		static const float TWO_PI;
		static const float HALF_PI;
		static const float fDeg2Rad;
		static const float fRad2Deg;

    };

	// these functions must be defined down here, because they rely on the
	// angle unit conversion functions in class Math:

	inline float Radian::valueDegrees() const
	{
		return Math::RadiansToDegrees ( mRad );
	}

	inline float Radian::valueAngleUnits() const
	{
		return Math::RadiansToAngleUnits ( mRad );
	}

	inline float Degree::valueRadians() const
	{
		return Math::DegreesToRadians ( mDeg );
	}

	inline float Degree::valueAngleUnits() const
	{
		return Math::DegreesToAngleUnits ( mDeg );
	}

	inline Angle::operator Radian() const
	{
		return Radian(Math::AngleUnitsToRadians(mAngle));
	}

	inline Angle::operator Degree() const
	{
		return Degree(Math::AngleUnitsToDegrees(mAngle));
	}

	inline Radian operator * ( float a, const Radian& b )
	{
		return Radian ( a * b.valueRadians() );
	}

	inline Radian operator / ( float a, const Radian& b )
	{
		return Radian ( a / b.valueRadians() );
	}

	inline Degree operator * ( float a, const Degree& b )
	{
		return Degree ( a * b.valueDegrees() );
	}

	inline Degree operator / ( float a, const Degree& b )
	{
		return Degree ( a / b.valueDegrees() );
	}
}


#endif
//EOF