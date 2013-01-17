#ifndef _FUNCTIONSPACEVECTOR_H_
#define _FUNCTIONSPACEVECTOR_H_

#include "Exception.h"
#include "FunctionSpace.h"

/**
    @interface FunctionSpaceVector
    @brief Common Interface of all Vectorial FunctionSpaces

    This is the @em common @em interface of
    all @em Vectorial FunctionSpaces.@n

    A FunctionSpaceVector can be @em interpolated,
    and can return a @em Local Basis associated
    to an Element of the Support.

    @note
    A VectorFunctionSpace is an @em interface, so it
    can't be instantiated.
*/


class FunctionSpaceVector : public FunctionSpace{
 public:
  virtual ~FunctionSpaceVector(void);

  virtual fullVector<double>
    interpolate(const MElement& element,
		const std::vector<double>& coef,
		const fullVector<double>& xyz) const = 0;

  virtual fullVector<double>
    interpolateInRefSpace(const MElement& element,
			  const std::vector<double>& coef,
			  const fullVector<double>& uvw) const = 0;

  void preEvaluateLocalFunctions(const fullMatrix<double>& point) const;
  void preEvaluateCurlLocalFunctions(const fullMatrix<double>& point) const;
  void preEvaluateDivLocalFunctions(const fullMatrix<double>& point) const;

  const fullMatrix<double>&
    getEvaluatedLocalFunctions(const MElement& element) const;

  const fullMatrix<double>&
    getEvaluatedCurlLocalFunctions(const MElement& element) const;

  const fullMatrix<double>&
    getEvaluatedDivLocalFunctions(const MElement& element) const;

  const fullMatrix<double>&
    getEvaluatedLocalFunctions(unsigned int orientation) const;

  const fullMatrix<double>&
    getEvaluatedCurlLocalFunctions(unsigned int orientation) const;

  const fullMatrix<double>&
    getEvaluatedDivLocalFunctions(unsigned int orientation) const;

 protected:
  FunctionSpaceVector(void);
};


/**
   @fn FunctionSpaceVector::~FunctionSpaceVector
   Deletes this FunctionSpaceVector
   **

   @fn FunctionSpaceVector::interpolate
   @param element The MElement to interpolate on
   @param coef The coefficients of the interpolation
   @param xyz The coordinate
   (of point @em inside the given @c element)
   of the interpolation in the @em Physical Space

   @return Returns the (vectorial) interpolated value

   @warning
   If the given coordinate are not in the given
   @c element @em Bad @em Things may happend

   @todo
   If the given coordinate are not in the given
   @c element @em Bad @em Things may happend
   ---> check
   **

   @fn FunctionSpaceVector::interpolateInRefSpace
   @param element The MElement to interpolate on
   @param coef The coefficients of the interpolation
   @param uvw The coordinate
   (of point @em inside the given @c element)
   of the interpolation in the @em Reference Space

   @return Returns the (vectorial) interpolated value

   @warning
   If the given coordinate are not in the given
   @c element @em Bad @em Things may happend

   @todo
   If the given coordinate are not in the given
   @c element @em Bad @em Things may happend
   ---> check
   **

   @fn FunctionSpaceVector::preEvaluateLocalFunctions
   @param point A set of @c 3D Points

   Precomputes the Local Functions of this FunctionSpace
   at the given Points.

   @note Each row of @c point is a new Point,
   and each column is a coordinate (for a total of
   3 columns)
   **

   @fn FunctionSpaceVector::preEvaluateCurlLocalFunctions
   @param point A set of @c 3D Points

   Precomputes the @em Curl of the Local Functions
   of this FunctionSpace at the given Points.

   @note Each row of @c point is a new Point,
   and each column is a coordinate (for a total of
   3 columns)
   **

   @fn FunctionSpaceVector::preEvaluateDivLocalFunctions
   @param point A set of @c 3D Points

   Precomputes the @em Divergence of the Local Functions
   of this FunctionSpace at the given Points.

   @note Each row of @c point is a new Point,
   and each column is a coordinate (for a total of
   3 columns)
   **

   @fn FunctionSpaceVector::getEvaluatedLocalFunctions(const MElement&) const
   @param element A MElement
   @return Returns the @em values of the @em precomputed
   Basis Functions associated
   to the given element (with correct @em closure)

   @note
   The returned values @em must be computed by
   FunctionSpaceVector::preEvaluateLocalFunctions(),
   if not an Exception will be thrown
   **

   @fn FunctionSpaceVector::getEvaluatedCurlLocalFunctions(const MElement&) const
   @param element A MElement
   @return Returns the @em values of the @em precomputed
   @em Curls of the Basis Functions associated
   to the given element (with correct @em closure)

   @note
   The returned values @em must be computed by
   FunctionSpaceVector::preEvaluateCurlLocalFunctions(),
   if not an Exception will be thrown
   **

   @fn FunctionSpaceVector::getEvaluatedDivLocalFunctions(const MElement&) const
   @param element A MElement
   @return Returns the @em values of the @em precomputed
   @em Divergences of the Basis Functions associated
   to the given element (with correct @em closure)

   @note
   The returned values @em must be computed by
   FunctionSpaceVector::preEvaluateDivLocalFunctions(),
   if not an Exception will be thrown
   **

   @fn FunctionSpaceVector::getEvaluatedLocalFunctions(unsigned int) const
   @param orientation A number definig the orientation of the reference space
   @return Same as
   FunctionSpaceVector::getEvaluatedLocalFunctions(const MElement&) const
   but the orientation is not given by en element but by a number (@c orientation)
   **

   @fn FunctionSpaceVector::getEvaluatedCurlLocalFunctions(unsigned int) const
   @param orientation A number definig the orientation of the reference space
   @return Same as
   FunctionSpaceVector::getEvaluatedCurlLocalFunctions(const MElement&) const
   but the orientation is not given by en element but by a number (@c orientation)
   **

   @fn FunctionSpaceVector::getEvaluatedDivLocalFunctions(unsigned int) const
   @param orientation A number definig the orientation of the reference space
   @return Same as
   FunctionSpaceVector::getEvaluatedDivLocalFunctions(const MElement&) const
   but the orientation is not given by en element but by a number (@c orientation)
*/

//////////////////////
// Inline Functions //
//////////////////////

inline void FunctionSpaceVector::
preEvaluateLocalFunctions(const fullMatrix<double>& point) const{
  localBasis->preEvaluateFunctions(point);
}

inline void FunctionSpaceVector::
preEvaluateCurlLocalFunctions(const fullMatrix<double>& point) const{
  localBasis->preEvaluateDerivatives(point);
}

inline void FunctionSpaceVector::
preEvaluateDivLocalFunctions(const fullMatrix<double>& point) const{
  localBasis->preEvaluateDerivatives(point);
}

inline const fullMatrix<double>&
FunctionSpaceVector::getEvaluatedLocalFunctions(const MElement& element) const{
  try{
    return localBasis->getPreEvaluatedFunctions(element);
  }

  catch(Exception& any){
    throw Exception("Local Basis Functions not PreEvaluated");
  }
}

inline const fullMatrix<double>&
FunctionSpaceVector::getEvaluatedCurlLocalFunctions(const MElement& element) const{
  try{
    return localBasis->getPreEvaluatedDerivatives(element);
  }

  catch(Exception& any){
    throw Exception("Curl of Local Basis Functions not PreEvaluated");
  }
}

inline const fullMatrix<double>&
FunctionSpaceVector::getEvaluatedDivLocalFunctions(const MElement& element) const{
  try{
    return localBasis->getPreEvaluatedFunctions(element);
  }

  catch(Exception& any){
    throw Exception("Divergence of Local Basis Functions not PreEvaluated");
  }
}

inline const fullMatrix<double>&
FunctionSpaceVector::getEvaluatedLocalFunctions(unsigned int orientation) const{
  try{
    return localBasis->getPreEvaluatedFunctions(orientation);
  }

  catch(Exception& any){
    throw Exception("Local Basis Functions not PreEvaluated");
  }
}

inline const fullMatrix<double>&
FunctionSpaceVector::getEvaluatedCurlLocalFunctions(unsigned int orientation) const{
  try{
    return localBasis->getPreEvaluatedDerivatives(orientation);
  }

  catch(Exception& any){
    throw Exception("Curl of Local Basis Functions not PreEvaluated");
  }
}

inline const fullMatrix<double>&
FunctionSpaceVector::getEvaluatedDivLocalFunctions(unsigned int orientation) const{
  try{
    return localBasis->getPreEvaluatedFunctions(orientation);
  }

  catch(Exception& any){
    throw Exception("Divergence of Local Basis Functions not PreEvaluated");
  }
}

#endif
