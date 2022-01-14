/**
 * \file imperative/src/include/megbrain/imperative/operator.h
 * MegEngine is Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Copyright (c) 2014-2021 Megvii Inc. All rights reserved.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT ARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#pragma once

#include <list>
#include <map>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <vector>

#include "megbrain/common.h"
#include "megbrain/imperative/utils/span.h"
#include "megbrain/imperative/value.h"

namespace mgb {
namespace imperative {

/**
 * \brief base class for all operators
 *
 */
class Operator {
public:
    enum Kind {
        IdentityLike,  // one input, one output, output is like input
        GetAttrLike,   // no tensor output
        Other,
    };

private:
    size_t m_typecode;
    Kind m_kind;

protected:
    Operator(size_t typecode, Kind kind) : m_typecode{typecode}, m_kind{kind} {}

public:
    size_t typecode() const { return m_typecode; }
    Kind kind() const { return m_kind; }

    template <typename U>
    U* as() const {
        if (m_typecode != U::TYPE_CODE) {
            return nullptr;
        }
        return static_cast<U*>(const_cast<Operator*>(this));
    }
    template <typename U>
    bool is() const {
        return as<U>() != nullptr;
    }
    template <Kind kKind>
    bool is() const {
        return kind() == kKind;
    }
    template <typename U>
    U& cast() const {
        U* ptr = as<U>();
        mgb_assert(ptr);
        return *ptr;
    }

    virtual std::string to_string() const = 0;

    /**
     * \brief fallback implementation of this. Not all operators has fallback
     * implementation.
     *
     * \param inputs
     * \return std::vector<ValueRef>
     */
    virtual std::vector<ValueRef> fallback(Span<ValueRef> inputs) const;

    std::type_index type() const { return registered_types()[m_typecode]; }

    static size_t register_type(std::type_index type);
    static const std::vector<std::type_index>& registered_types();
};

template <typename T, Operator::Kind kKind = Operator::Other>
class OperatorImpl : public Operator {
protected:
    OperatorImpl() : Operator(TYPE_CODE, kKind) {}

public:
    static inline size_t TYPE_CODE = [] { return register_type(typeid(T)); }();

    std::string to_string() const override = 0;
};

}  // namespace imperative
}  // namespace mgb