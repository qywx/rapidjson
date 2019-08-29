// Tencent is pleased to support the open source community by making RapidJSON available.
// 
// Copyright (C) 2017 THL A29 Limited, a Tencent company, and Milo Yip, and Ivan Kuvaldin. All rights reserved.
//
// Licensed under the MIT License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, software distributed 
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
// CONDITIONS OF ANY KIND, either express or implied. See the License for the 
// specific language governing permissions and limitations under the License.

#include "rapidjson.h"
#include "rj/include/rapidjson/stringbuffer.h"
#ifndef RAPIDJSON_PointedValueHandler_H_
#define RAPIDJSON_PointedValueHandler_H_
#pragma once


#include "pointer.h"
#include "reader.h"
#include <list>

RAPIDJSON_NAMESPACE_BEGIN

   /*! \class rapidjson::PointedValueHandler
       \brief Concept for receiving events from GenericReader upon parsing.
       The functions return true if no error occurs. If they return false,
       the event publisher should terminate the process.
   \code
   concept PointedValueHandler
   {
   public:  // == TYPES ==
       using Encoding = UTF8<>;
      using Ch = Encoding::Ch;
   public:  // == METHODS ==
      bool Default(Pointer const& p);
      bool Null( Pointer const& p );
      bool Bool(Pointer const& p, bool b);
      bool Int(Pointer const& p, int i);
      bool Uint(Pointer const& p, unsigned i);
      bool Int64(Pointer const& p, int64_t i);
      bool Uint64(Pointer const& p, uint64_t i);
      bool Double(Pointer const& p, double d);
      bool RawNumber(Pointer const& p, const Ch* str, SizeType length, bool copy);
      bool String(Pointer const& p, const Ch* str, SizeType length, bool copy);
      bool StartObject(Pointer const& p);
      bool EndObject(Pointer const& p, SizeType memberCount);
      bool StartArray(Pointer const& p);
      bool EndArray(Pointer const& p, SizeType elementCount);
   };
   \endcode
   */

   //! Default (dummy) implementation of PointedValueHandler concept.
   /*! This can be used as base class of any pointed reader handler.
       The sens of using it is no need to implement all methods of concept. 
       Non-overriden (read non-hidden) methods will fall back to method \c Default(),
       which is also could be overridden-hidden.
       \note analogy to BaseReaderHandler
       \usage class MyPointedHandler : public DummyPointedValueHandler
    */
   template<typename Encoding_ = UTF8<>, typename Derived_ = void>
   struct DummyPointedValueHandler
   {
   public:  // == TYPES == 
      using Encoding = Encoding_;
      using Derived = Derived_;
      using ThisT = DummyPointedValueHandler<Encoding,Derived>;
      using Ch = typename Encoding::Ch;
      // If Derived == void -> Override = ThisT, else Override = Derived
      using Override = typename internal::SelectIf<internal::IsSame<Derived, void>, ThisT, Derived>::Type;
   public:  // == METHODS ==
      /// Default is do nothing
      bool Default(Pointer const& p)                         { (void)p; return true; }
      bool Null   (Pointer const& p)                         { return static_cast<Override&>(*this).Default(p); }
      bool Bool   (Pointer const& p, bool b)                 { return static_cast<Override&>(*this).Default(p,b); }
      bool Int    (Pointer const& p, int i)                  { return static_cast<Override&>(*this).Default(p,i); }
      bool Uint   (Pointer const& p, unsigned i)             { return static_cast<Override&>(*this).Default(p,i); }
      bool Int64  (Pointer const& p, int64_t i)              { return static_cast<Override&>(*this).Default(p,i); }
      bool Uint64 (Pointer const& p, uint64_t i)             { return static_cast<Override&>(*this).Default(p,i); }
      bool Double (Pointer const& p, double d)               { return static_cast<Override&>(*this).Default(p,d); }
      /// Enabled via kParseNumbersAsStringsFlag, string is not null-terminated (use length)
      bool RawNumber(Pointer const& p, const Ch* str, SizeType length, bool copy) { return static_cast<Override&>(*this).String(p,str,length,copy); }
      bool String   (Pointer const& p, const Ch* str, SizeType length, bool copy) { return static_cast<Override&>(*this).Default(p); }
      bool StartObject(Pointer const& p)                                          { return static_cast<Override&>(*this).Default(p); }
      //bool Key(const Ch* str, SizeType length, bool copy)                       { return static_cast<Override&>(*this).String(p,str,length,copy); }
      bool EndObject(Pointer const& p, SizeType memberCount)                      { return static_cast<Override&>(*this).Default(p); }
      bool StartArray(Pointer const& p)                                           { return static_cast<Override&>(*this).Default(p); }
      bool EndArray(Pointer const& p, SizeType elementCount)                      { return static_cast<Override&>(*this).Default(p); }
   };


   /// Handler implements usual rapidjson concept `rapidjson::Handler`.
   /// Passes values with pointer to \tparam PointedValueHandler
   ///\tparam PointedValueHandler : a type implements concept PointedValueHandler
   ///\tparam Encoding  /see rapidjson/encodings.h
   template<typename PointedValueHandler /*= DummyPointedValueHandler<>*/, typename Encoding = UTF8<>>
   class KeyValueSaxHandler  //HandlerToPointedHandler
      // No need to inherit `BaseReaderHandler`, we simply repeat it with ALL methods.
      //: public BaseReaderHandler<Encoding, KeyValueSaxHandler<Encoding,PointedValueHandler>>
   {
   public:  // == TYPES == 
      //using Override = 
      //		typename internal::SelectIf<internal::IsSame<PointedValueHandler, void>, KeyValueSaxHandler, PointedValueHandler>::Type;
      using Ch = typename Encoding::Ch;
   public:
      //KeyValueSaxHandler( PointedValueHandler && pvh /*= {}*/ )
      //		: pvh_(pvh)
      //{}
      //KeyValueSaxHandler( PointedValueHandler & pvh )
      //		: pvh_(pvh)
      //{}
      KeyValueSaxHandler( PointedValueHandler & pvh, Pointer prepointer={} )
            : pvh_(pvh)
            , pointer_(prepointer)
      {}
      /*//template< typename = enable_if( type_traits::default_constructable<PointedValueHandler>::type ) >
      KeyValueSaxHandler()
            : pvh_{}
      {}*/
   public:  // == METHODS ==
      bool Default()                                          { bool ret = true;                    pointerDetach(); return ret; }
      bool Null()                                             { bool ret = pvh_.Null(pointer_);     pointerDetach(); return ret; }
      bool Bool(bool b)                                       { bool ret = pvh_.Bool(pointer_,b);   pointerDetach(); return ret; }
      bool Int(int i)                                         { bool ret = pvh_.Int(pointer_,i);    pointerDetach(); return ret; }
      bool Uint(unsigned u)                                   { bool ret = pvh_.Uint(pointer_,u);   pointerDetach(); return ret; }
      bool Int64(int64_t i)                                   { bool ret = pvh_.Int64(pointer_,i);  pointerDetach(); return ret; }
      bool Uint64(uint64_t u)                                 { bool ret = pvh_.Uint64(pointer_,u); pointerDetach(); return ret; }
      bool Double(double d)                                   { bool ret = pvh_.Double(pointer_,d); pointerDetach(); return ret; }
      /// enabled via kParseNumbersAsStringsFlag, string is not null-terminated (use length)
      bool RawNumber(const Ch* str, SizeType len, bool copy)  { bool ret = pvh_.RawNumber(pointer_,str,len,copy); pointerDetach(); return ret; }
      bool String   (const Ch* str, SizeType len, bool copy)
      {
            bool ret = pvh_.String(pointer_,str,len,copy);
            pointerDetach();
            return ret;
        }
      
      bool StartObject()
      {
            return pvh_.StartObject(pointer_);
        }

      bool EndObject(SizeType size)
      {
            bool ret = pvh_.EndObject(pointer_,size);
            pointerDetach();
            return ret;
        }
      
      bool Key(const Ch* str, SizeType length, bool copy){
         (void)copy;
         pointer_ = pointer_.Append(str,length);  //\todo Ask why this creates new object rather than modify current
         return true;
      }
      
      bool StartArray()
      {
         bool ret = pvh_.StartArray(pointer_);

         pointer_ = pointer_.Append(0); // "/array/" -> "/array/0/"
         arrays_.push_back(pointer_);

         return ret;
      }

      bool EndArray(SizeType size)
      {
        pointer_.Detach(); // "/array/0/" -> "/array/"
        arrays_.pop_back();

        bool ret = pvh_.EndArray(pointer_,size);

        pointerDetach();

        return ret;
      }
      
   protected:
      PointedValueHandler & pvh_;
      Pointer pointer_ = {};
      std::list<Pointer> arrays_;

   protected:
      /**
       * @brief Elements of JSON Arrays has no own key tokens.
       * So we add "0/" token to pointer_ for all elements in StartArray(), and remove them in EndArray().
       * In other cases we add token in Key() and must detach last tokens here.
       *
       * Note: all elements in array has '0' index.
       * This allow check hierarchy and types from MasterDoc, which has only one example element.
       */
      Pointer & pointerDetach()
      {
         if(arrays_.empty() || arrays_.back() != pointer_)
         {
           pointer_.Detach();
         }

         return pointer_;
      }
   };
   
   
   /// Attempt to implement Handler, which should be derived.
   ///\usage `class MyPointedValueHandlerImpl : public PointedHandlerBase<UTF8<>,MyPointedValueHandlerImpl>`
   template< typename Encoding_ = UTF8<>, typename Derived_ = DummyPointedValueHandler<Encoding_> >
   class PointedHandlerBase
      // Inherit dummy implementation of `PointedValueHandler` concept 
      //: DummyPointedValueHandler<Encoding_,Derived_>
      // No need to inherit `BaseReaderHandler`, we simply repeat it with ALL methods, in other words implement concept `rapidjson::Handler`.
      //: public BaseReaderHandler<Encoding, KeyValueSaxHandler<Encoding,PointedValueHandler>>
   {
   public:  // == TYPES == 
      using Encoding = Encoding_;
      using Derived = Derived_;
      using ThisT = PointedHandlerBase<Encoding,Derived>;
      using Ch = typename Encoding::Ch;
      // If Derived was provided Override = Derived, else Override = ThisT 
      using Override = typename internal::SelectIf<internal::IsSame<Derived, void>, ThisT, Derived>::Type;
   public:  // == METHODS ==
      bool Default()                                          { bool ret = override().Default(pointer_)  ; pointer_.Detach(); return ret; }
      bool Null()                                             { bool ret = override().Null   (pointer_)  ; pointer_.Detach(); return ret; }
      bool Bool(bool b)                                       { bool ret = override().Bool   (pointer_,b); pointer_.Detach(); return ret; }
      bool Int(int i)                                         { bool ret = override().Int    (pointer_,i); pointer_.Detach(); return ret; }
      bool Uint(unsigned u)                                   { bool ret = override().Uint   (pointer_,u); pointer_.Detach(); return ret; }
      bool Int64(int64_t i)                                   { bool ret = override().Int64  (pointer_,i); pointer_.Detach(); return ret; }
      bool Uint64(uint64_t u)                                 { bool ret = override().Uint64 (pointer_,u); pointer_.Detach(); return ret; }
      bool Double(double d)                                   { bool ret = override().Double (pointer_,d); pointer_.Detach(); return ret; }
      /// enabled via kParseNumbersAsStringsFlag, string is not null-terminated (use length)
      bool RawNumber(const Ch* str, SizeType len, bool copy)  { bool ret = override().RawNumber(pointer_,str,len,copy); pointer_.Detach(); return ret; }
      bool String   (const Ch* str, SizeType len, bool copy)  { bool ret = override().String   (pointer_,str,len,copy); pointer_.Detach(); return ret; }
      
      bool StartObject()                                      { return override().StartObject(pointer_); }
      //bool Key(const Ch* str, SizeType len, bool copy) { return static_cast<Override&>(*this).String(str, len, copy); }
      bool EndObject(SizeType size)                           { bool ret = override().EndObject(pointer_,size); pointer_.Detach(); return ret; }
      bool StartArray()                                       { return override().StartArray(pointer_); }
      bool EndArray(SizeType size)                            { bool ret = override().EndArray(pointer_,size); pointer_.Detach(); return ret; }
   public:	
      bool Key(const Ch* str, SizeType length, bool copy){
         (void)copy;
         pointer_ = pointer_.Append(str,length);  //\todo Ask why this creates new object rather than modify current
         return true;
      }
   public:
      bool Default(Pointer const& p)                         { (void)p; return true; }
   public:
      constexpr Override& override(){ return static_cast<Override&>(*this); }
   protected:
      Pointer pointer_ = {};
   };
   
RAPIDJSON_NAMESPACE_END

#endif // RAPIDJSON_PointedValueHandler_H_
