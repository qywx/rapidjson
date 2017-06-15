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
	
RAPIDJSON_NAMESPACE_END

#endif // RAPIDJSON_ToStdStream_H_
