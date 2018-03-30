/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#pragma once

#include "ValueDef.h"
#include "XMLRefFactory.h"
#include "XMLValueRef.h"
#include "ValueOptions.h"

namespace Oak {
namespace Model {

class ValueDefBuilder;
typedef std::shared_ptr<ValueDefBuilder> ValueDefBuilderSPtr;
typedef std::weak_ptr<ValueDefBuilder> ValueDefBuilderWPtr;

// =============================================================================
// Class definition
// =============================================================================
class ValueDefBuilder
{
protected:
    ValueDefBuilder(UnionType type, const std::string &name);
    ValueDefBuilder(ValueDef *valueDef);

public:
    static ValueDefBuilderSPtr create(UnionType type, const std::string &name);
    static ValueDefBuilderSPtr use(ValueDef *valueDef);

    ValueDefBuilderSPtr setName(const std::string &value);
    ValueDefBuilderSPtr setDisplayName(const std::string &value);
    ValueDefBuilderSPtr setTooltip(const std::string &value);


    ValueDefBuilderSPtr setDefaultValue(const UnionRef &value);

    ValueDefBuilderSPtr setDefaultConversion(ConversionSPtr value);

    ValueDefBuilderSPtr addOptionStatic(const UnionRef & value);
    template<typename T>
    ValueDefBuilderSPtr setOptionsStatic(const std::vector<T> &options);
    template<typename T>
    ValueDefBuilderSPtr setOptionsExcludedStatic(const std::vector<T> &options);

    ValueDefBuilderSPtr setOptionsQuery(QueryRefSPtr value);
    ValueDefBuilderSPtr setOptionsExcludedQuery(QueryRefSPtr value);

    ValueDefBuilderSPtr setSetting(std::string name, bool value);

#ifdef XML_BACKEND
    ValueDefBuilderSPtr setXMLValueRef(XML::ValueRefUPtr value);
#endif
    const ValueDef &valueDef() const;
    ValueDefUPtr get();

private:
    ValueDefUPtr m_valueDefUPtr;
    ValueDef *m_valueDef;

    ValueDefBuilderWPtr m_thisWPtr;
};

// =============================================================================
// (public)
template<typename T>
ValueDefBuilderSPtr ValueDefBuilder::setOptionsStatic(const std::vector<T> &options)
{
    assert(m_valueDef);
    if (!options.empty() && m_valueDef->m_valueTemplate.type() == UnionValue::GetType(options.front())) {
        m_valueDef->m_options->m_options.clear();
        for (const auto& option: options)
        {
            m_valueDef->m_options->m_options.push_back(option);
        }
    } else {
        assert(false);
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
template<typename T>
ValueDefBuilderSPtr ValueDefBuilder::setOptionsExcludedStatic(const std::vector<T> &options)
{
    assert(m_valueDef);
    if (!options.empty() && m_valueDef->m_valueTemplate.type() == UnionValue::GetType(options.front())) {
        m_valueDef->m_options->m_excluded.clear();
        for (const auto& option: options)
        {
            m_valueDef->m_options->m_excluded.push_back(option);
        }
    } else {
        assert(false);
    }
    return m_thisWPtr.lock();
}

typedef ValueDefBuilder VDB;

} // namespace Model
} // namespace Oak
