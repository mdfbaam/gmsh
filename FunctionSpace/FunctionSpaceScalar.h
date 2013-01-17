#ifndef _FUNCTIONSPACESCALAR_H_
#define _FUNCTIONSPACESCALAR_H_

#include "Exception.h"
#include "FunctionSpace.h"

/**
    @interface FunctionSpaceScalar
    @brief Common Interface of all Scalar FunctionSpaces

    This is the @em common @em interface of
    all @em Scalar FunctionSpaces.@n

    A FunctionSpaceScalar can be @em interpolated,
    and can return a @em Local Basis associated
    to an Element of the Support.

    @note
    A ScalarFunctionSpace is an @em interface, so it
    can't be instantiated.
*/


class FunctionSpaceScalar : public FunctionSpace{
 public:
  virtual ~FunctionSpaceScalar(void);

  virtual double
    interpolate(const MElement& element,
		const std::vector<double>& coef,
		const fullVector<double>& xyz) const = 0;

  virtual double
    interpolateInRefSpace(const MElement& element,
			  const std::vector<double>& coef,
			  const fullVector<double>& uvw) const = 0;

  void preEvaluateLocalFunctions(const fullMatrix<double>& point) const;
  void preEvaluateGradLocalFunctions(const fullMatrix<double>& point) const;

  const fullMatrix<double>&
    getEvaluatedLocalFunctions(const MElement& element) const;

  const fullMatrix<double>&
    getEvaluatedGradLocalFunctions(const MElement& element) const;

  const fullMatrix<double>&
    getEvaluatedLocalFunctions(unsigned int orientation) const;

  const fullMatrix<double>&
    getEvaluatedGradLocalFunctions(unsigned int orientation) const;

 protected:
  FunctionSpaceScalar(void);
};


/**
   @fn FunctionSpaceScalar::~FunctionSpaceScalar
   Deletes this FunctionSpaceScalar
   **

   @fn FunctionSpaceScalar::interpolate
   @param element The MElement to interpolate on
   @param coef The coefficients of the interpolation
   @param xyz The coordinate
   (of point @em inside the given @c element)
   of the interpolation in the @em Physical Space

   @return Returns the (scalar) interpolated value

   @warning
   If the given coordinate are not in the given
   @c element @em Bad @em Things may happend

   @todo
   If the given coordinate are not in the given
   @c element @em Bad @em Things may happend
   ---> check
   **

   @fn FunctionSpaceScalar::interpolateInRefSpace
   @param element The MElement to interpolate on
   @param coef The coefficients of the interpolation
   @param uvw The coordinate
   (of point @em inside the given @c element)
   of the interpolation in the @em Reference Space

   @return Returns the (scalar) interpolated value

   @warning
   If the given coordinate are not in the given
   @c element @em Bad @em Things may happend

   @todo
   If the given coordinate are not in the given
   @c element @em Bad @em Things may happend
   ---> check
   **

   @fn FunctionSpaceScalar::preEvaluateLocalFunctions
   @param point A set of @c 3D Points

   Precomputes the Local Functions of this FunctionSpace
   at the given Points.

   @note Each row of @c point is a new Point,
   and each column is a coordinate (for a total of
   3 columns)
   **

   @fn FunctionSpaceScalar::preEvaluateGradLocalFunctions
   @param point A set of @c 3D Points

   Precomputes the @em Gradient of the Local Functions
   of this FunctionSpace at the given Points.

   @note Each row of @c point is a new Point,
   and each column is a coordinate (for a total of
   3 columns)
   **

   @fn FunctionSpaceScalar::getEvaluatedLocalFunctions(const MElement&) const
   @param element A MElement
   @return Returns the @em values of the @em precomputed
   Basis Functions associated
   to the given element (with correct @em closure)

   @note
   The returned values @em must be computed by
   FunctionSpaceScalar::preEvaluateLocalFunctions(),
   if not an Exception will be thrown
   **

   @fn FunctionSpaceScalar::getEvaluatedGradLocalFunctions(const MElement&) const
   @param element A MElement
   @return Returns the @em values of the @em precomputed
   @em Gradients of the Basis Functions associated
   to the given element (with correct @em closure)

   @note
   The returned values @em must be computed by
   FunctionSpaceScalar::preEvaluateGradLocalFunctions(),
   if not an Exception will be thrown
   **

   @fn FunctionSpaceScalar::getEvaluatedLocalFunctions(unsigned int) const
   @param orientation A number definig the orientation of the reference space
   @return Same as
   FunctionSpaceScalar::getEvaluatedLocalFunctions(const MElement&) const
   but the orientation is not given by en element but by a number (@c orientation)
   **

   @fn FunctionSpaceScalar::getEvaluatedGradLocalFunctions(unsigned int) const
   @param orientation A number definig the orientation of the reference space
   @return Same as
   FunctionSpaceScalar::getEvaluatedGradLocalFunctions(const MElement&) const
   but the orientation is not given by en element but by a number (@c orientation)
*/

//////////////////////
// Inline Functions //
//////////////////////

inline void FunctionSpaceScalar::
preEvaluateLocalFunctions(const fullMatrix<double>& point) const{
  localBasis->preEvaluateFunctions(point);
}

inline void FunctionSpaceScalar::
preEvaluateGradLocalFunctions(const fullMatrix<double>& point) const{
  localBasis->preEvaluateDerivatives(point);
}

inline const fullMatrix<double>&
FunctionSpaceScalar::getEvaluatedLocalFunctions(const MElement& element) const{
  try{
    return localBasis->getPreEvaluatedFunctions(element);
  }

  catch(Exception& any){
    throw Exception("Local Basis Functions not PreEvaluated");
  }
}

inline const fullMatrix<double>&
FunctionSpaceScalar::getEvaluatedGradLocalFunctions(const MElement& element) const{
  try{
    return localBasis->getPreEvaluatedDerivatives(element);
  }

  catch(Exception& any){
    throw Exception("Gradient of Local Basis Functions not PreEvaluated");
  }
}

inline const fullMatrix<double>&
FunctionSpaceScalar::getEvaluatedLocalFunctions(unsigned int orientation) const{
  try{
    return localBasis->getPreEvaluatedFunctions(orientation);
  }

  catch(Exception& any){
    throw Exception("Local Basis Functions not PreEvaluated");
  }
}

inline const fullMatrix<double>&
FunctionSpaceScalar::getEvaluatedGradLocalFunctions(unsigned int orientation) const{
  try{
    return localBasis->getPreEvaluatedDerivatives(orientation);
  }

  catch(Exception& any){
    throw Exception("Gradient of Local Basis Functions not PreEvaluated");
  }
}

#endif
