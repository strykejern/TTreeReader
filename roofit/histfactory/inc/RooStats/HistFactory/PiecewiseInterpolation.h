/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: PiecewiseInterpolation.h,v 1.3 2007/05/11 09:11:30 verkerke Exp $
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/
#ifndef ROO_PIECEWISEINTERPOLATION
#define ROO_PIECEWISEINTERPOLATION

#include "RooAbsReal.h"
#include "RooRealProxy.h"
#include "RooListProxy.h"

#include "RooObjCacheManager.h"

class RooRealVar;
class RooArgList ;

class PiecewiseInterpolation : public RooAbsReal {
public:

  PiecewiseInterpolation() ;
  PiecewiseInterpolation(const char *name, const char *title, const RooAbsReal& nominal, const RooArgList& lowSet, const RooArgList& highSet, const RooArgList& paramSet, Bool_t takeOwnerShip=kFALSE) ;
  virtual ~PiecewiseInterpolation() ;

  PiecewiseInterpolation(const PiecewiseInterpolation& other, const char* name = 0);
  virtual TObject* clone(const char* newname) const { return new PiecewiseInterpolation(*this, newname); }

  //  virtual Double_t defaultErrorLevel() const ;

  //  void printMetaArgs(ostream& os) const ;

  const RooArgList& lowList() const { return _lowSet ; }
  const RooArgList& highList() const { return _highSet ; }
  const RooArgList& paramList() const { return _paramSet ; }

  virtual Bool_t forceAnalyticalInt(const RooAbsArg&) const { return kTRUE ; }

  Int_t getAnalyticalIntegralWN(RooArgSet& allVars, RooArgSet& analVars, const RooArgSet* normSet,const char* rangeName=0) const ;
  Double_t analyticalIntegralWN(Int_t code, const RooArgSet* normSet, const char* rangeName=0) const ;

  void setPositiveDefinite(bool flag=true){_positiveDefinite=flag;}
protected:

  class CacheElem : public RooAbsCacheElement {
  public:
    CacheElem()  {} ;
    virtual ~CacheElem() {} ; 
    virtual RooArgList containedArgs(Action) { 
      RooArgList ret(_funcIntList) ; 
      ret.add(_lowIntList); 
      ret.add(_highIntList);
      return ret ; 
    }
    RooArgList _funcIntList ;
    RooArgList _lowIntList ;
    RooArgList _highIntList ;
    // will want vector<RooRealVar*> for low and high also
  } ;
  mutable RooObjCacheManager _normIntMgr ; // The integration cache manager

  RooRealProxy _nominal;           // The nominal value
  RooArgList   _ownedList ;       // List of owned components
  RooListProxy _lowSet ;            // Low-side variation
  RooListProxy _highSet ;            // High-side varaition
  RooListProxy _paramSet ;            // interpolation parameters
  mutable TIterator* _paramIter ;  //! Iterator over paramSet
  mutable TIterator* _lowIter ;  //! Iterator over lowSet
  mutable TIterator* _highIter ;  //! Iterator over highSet
  Bool_t _positiveDefinite; // protect against negative and 0 bins.

  Double_t evaluate() const;

  ClassDef(PiecewiseInterpolation,2) // Sum of RooAbsReal objects
};

#endif