// spacy.cpp
//
// URL:      https://github.com/d99kris/spacy-cpp
//
// Copyright (C) 2017 Kristofer Berggren
// All rights reserved.
//
// spacy-cpp is distributed under the MIT license, see LICENSE for details.

#include "spacy.h"

#include <memory>
#include <string>

#include "attrs.h"
#include "nlp.h"
#include "pyobjectptr.h"
#include "python.h"

namespace Spacy
{
  Spacy::Spacy()
    : m_python(Python::get_instance())
    , m_spacy(Python::import("spacy"))
  {
    if (m_spacy.get() == nullptr)
    {
      throw std::runtime_error("No module named spacy. Try: pip install -U spacy");
    }
  }

  Spacy::~Spacy()
  {
  }
    
  Nlp Spacy::load(const std::string& p_model)
  {
    PyObjectPtr model(Python::get_object<std::string>(p_model));
    std::vector<PyObjectPtr> args = std::vector<PyObjectPtr>({model});
    PyObjectPtr nlp(Python::call_method<PyObjectPtr>(m_spacy, "load", args));
    if (nlp.get() == nullptr)
    {
      throw std::runtime_error("Can't find model '" + p_model + "'. "
                               "Try: python -m spacy download " + p_model);
    }
    return Nlp(nlp);
  }

  const Attrs& Spacy::attrs()
  {
    if (!m_attrs)
    {
      PyObjectPtr attrs(Python::get_attr_value<PyObjectPtr>(m_spacy, "attrs"));
      m_attrs.reset(new Attrs(attrs));
    }
    return *m_attrs;
  }
}
