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

#ifndef RAPIDJSON_PointedValueHandler_H_
#define RAPIDJSON_PointedValueHandler_H_
 
#include "pointer.h"
#include "reader.h"

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

	//! Default implementation of PointedValueHandler.
	/*! This can be used as base class of any pointed reader handler.
	    The sens of using it is no need to implement all methods of concept. 
	    Non-overriden (read non-hidden) methods will fall back to method \c Default() 
	    \note implements PointedValueHandler concept
	    \note analogy to BaseReaderHandler
	 */
	template<typename Encoding_ = UTF8<>, typename Derived_ = void>
	struct BasePointedValueHandler
	{
	public:  // == TYPES == 
		using Encoding = Encoding_;
		using Derived = Derived_;
		using ThisT = BasePointedValueHandler<Encoding,Derived>;
		using Ch = typename Encoding::Ch;
		// If Derived == void -> Override = ThisT, else Override = Derived
		using Override = typename internal::SelectIf<internal::IsSame<Derived, void>, ThisT, Derived>::Type;
	public:  // == METHODS ==
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


	/// Handler implements usual rapidjson concept.
	/// Passes values with pointer to \tparam PointedValueHandler
	///\tparam PointedValueHandler implements concept PointedValueHandler
	///\tparam Encoding
	template<typename PointedValueHandler /*= BasePointedValueHandler<>*/, typename Encoding = UTF8<>>
	class KeyValueSaxHandler
		// No need to inherit `BaseReaderHandler`, we simply repeat it with ALL methods.
		//: public BaseReaderHandler<Encoding, KeyValueSaxHandler<Encoding,PointedValueHandler>>
	{
	public:  // == TYPES == 
		//using Override = 
		//		typename internal::SelectIf<internal::IsSame<PointedValueHandler, void>, KeyValueSaxHandler, PointedValueHandler>::Type;
		using Ch = typename Encoding::Ch;
	public:  // == METHODS ==
		bool Default()                                          { bool ret = true;                    pointer_.Detach(); return ret; }
		bool Null()                                             { bool ret = pvh_.Null(pointer_);     pointer_.Detach(); return ret; }
		bool Bool(bool b)                                       { bool ret = pvh_.Bool(pointer_,b);   pointer_.Detach(); return ret; }
		bool Int(int i)                                         { bool ret = pvh_.Int(pointer_,i);    pointer_.Detach(); return ret; }
		bool Uint(unsigned u)                                   { bool ret = pvh_.Uint(pointer_,u);   pointer_.Detach(); return ret; }
		bool Int64(int64_t i)                                   { bool ret = pvh_.Int64(pointer_,i);  pointer_.Detach(); return ret; }
		bool Uint64(uint64_t u)                                 { bool ret = pvh_.Uint64(pointer_,u); pointer_.Detach(); return ret; }
		bool Double(double d)                                   { bool ret = pvh_.Double(pointer_,d); pointer_.Detach(); return ret; }
		/// enabled via kParseNumbersAsStringsFlag, string is not null-terminated (use length)
		bool RawNumber(const Ch* str, SizeType len, bool copy)  { bool ret = pvh_.RawNumber(pointer_,str,len,copy); pointer_.Detach(); return ret; }
		bool String   (const Ch* str, SizeType len, bool copy)  { bool ret = pvh_.String(pointer_,str,len,copy);    pointer_.Detach(); return ret; }
		
		bool StartObject()                                      { return pvh_.StartObject(pointer_); }
		//bool Key(const Ch* str, SizeType len, bool copy) { return static_cast<Override&>(*this).String(str, len, copy); }
		bool EndObject(SizeType size)                           { bool ret = pvh_.EndObject(pointer_,size); pointer_.Detach(); return ret; }
		bool StartArray()                                       { return pvh_.StartArray(pointer_); }
		bool EndArray(SizeType size)                            { bool ret = pvh_.EndArray(pointer_,size); pointer_.Detach(); return ret; }
		
		bool Key(const Ch* str, SizeType length, bool copy){
			(void)copy;
			pointer_ = pointer_.Append(str,length);  //\todo Ask why this creates new object rather than modify current
			return true;
		}
	protected:
		PointedValueHandler pvh_ = {};
		Pointer pointer_ = {};
	};

	
RAPIDJSON_NAMESPACE_END

#endif // RAPIDJSON_PointedValueHandler_H_
