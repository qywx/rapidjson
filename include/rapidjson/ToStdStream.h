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

#ifndef RAPIDJSON_ToStdStream_H_
#define RAPIDJSON_ToStdStream_H_

#include "pointer.h"
//#include "stringbuffer.h"
#include "ostreamwrapper.h"
#include "writer.h"
#include "prettywriter.h"
#include <iostream>


RAPIDJSON_NAMESPACE_BEGIN
	
	/*/// Stringify to accustomed `std::ostream`
	std::ostream& operator<<( std::ostream &os, Pointer const& p ){
		const size_t n = p.GetTokenCount();
		const Token* t = p.GetTokens();
		for( size_t i = 0; i<n; ++i )
			os << '/' << t[i].name;
		return os;
	}*/
	
	/// Stringify to accustomed `std::ostream`
	template <typename ValueType, typename Allocator = CrtAllocator>
	std::ostream& operator<<( std::ostream &os, GenericPointer<ValueType,Allocator> const& p ){
		const size_t n = p.GetTokenCount();
		const auto *t = p.GetTokens();  //const typename GenericPointer<ValueType,Allocator>::Token *t = p.GetTokens();
		for( size_t i = 0; i<n; ++i )
			os << '/' << t[i].name;
		return os;
	}
	
	/*template <typename ValueT>
	std::ostream& operator<<( std::ostream &os, ValueT const& v ){
		switch( v->GetType() ){ 
			case kNullType:   os << "null";
			case kFalseType:  os << "false";
			case kTrueType:   os << "true";
			case kObjectType: os << "{...}";  //todo recursively
			case kArrayType:  os << "[...]]";  //todo recursively
			case kStringType: os << v.GetString();
			case kNumberType: os << v.GetNumber();
		}
		return os;
	}*/
	
	
	//std::ostream& operator<<( std::ostream &os, Type t ){
	template<typename _CharT, typename _Traits>
	std::basic_ostream<_CharT, _Traits>& operator<<
	( std::basic_ostream<_CharT, _Traits> &os, Type t ){
		switch(t){
			//case kNullType  : os << RAPIDJSON_STRINGIFY(kNullType);   break;  //!< null
			//case kFalseType : os << RAPIDJSON_STRINGIFY(kFalseType);  break;  //!< false
			//case kTrueType  : os << RAPIDJSON_STRINGIFY(kTrueType);   break;  //!< true
			//case kObjectType: os << RAPIDJSON_STRINGIFY(kObjectType); break;  //!< object
			//case kArrayType : os << RAPIDJSON_STRINGIFY(kArrayType);  break;  //!< array 
			//case kStringType: os << RAPIDJSON_STRINGIFY(kStringType); break;  //!< string
			//case kNumberType: os << RAPIDJSON_STRINGIFY(kNumberType); break;  //!< number
			case kNullType  : os << "Null"  ; break;  //!< null
			case kFalseType : os << "Bool"  ; break;  //!< false
			case kTrueType  : os << "Bool"  ; break;  //!< true
			case kObjectType: os << "Object"; break;  //!< object
			case kArrayType : os << "Array" ; break;  //!< array 
			case kStringType: os << "String"; break;  //!< string
			case kNumberType: os << "Number"; break;  //!< number
		}
		return os;
	}
	
	template<typename OutputStream>
	using PrettyWriterWithNanAndInf =
	PrettyWriter<
		/*typename OutputStream  */ OutputStream,
		/*typename SourceEncoding*/ UTF8<>,
		/*typename TargetEncoding*/ UTF8<>,
		/*typename StackAllocator*/ CrtAllocator,
		/*unsigned writeFlags*/     kWriteNanAndInfFlag
		>;
		
	template<typename OutputStream>
	using WriterWithNanAndInf =
	Writer<
		/*typename OutputStream  */ OutputStream,
		/*typename SourceEncoding*/ UTF8<>,
		/*typename TargetEncoding*/ UTF8<>,
		/*typename StackAllocator*/ CrtAllocator,
		/*unsigned writeFlags*/     kWriteNanAndInfFlag
		>;
	
	
	template< typename _CharT, typename _Traits  
			  , typename Encoding, typename Allocator=MemoryPoolAllocator<>, typename StackAllocator=CrtAllocator >
	std::basic_ostream<_CharT, _Traits>& operator<<
	//( std::basic_ostream<_CharT, _Traits> &os, Document const& doc ){
	( std::basic_ostream<_CharT, _Traits> &os, GenericDocument<Encoding,Allocator,StackAllocator> const& doc )
	{
		OStreamWrapper osw(os);
		WriterWithNanAndInf<OStreamWrapper> writer(osw);
		doc.Accept(writer);
		return os;
	}
	
	
RAPIDJSON_NAMESPACE_END

#endif // RAPIDJSON_ToStdStream_H_
