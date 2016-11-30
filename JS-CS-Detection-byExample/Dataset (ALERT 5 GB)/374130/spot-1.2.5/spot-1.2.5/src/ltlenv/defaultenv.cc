// -*- coding: utf-8 -*-
// Copyright (C) 2012 Laboratoire de Recherche et Développement
// de l'Epita (LRDE).
// Copyright (C) 2003  Laboratoire d'Informatique de Paris 6 (LIP6),
// département Systèmes Répartis Coopératifs (SRC), Université Pierre
// et Marie Curie.
//
// This file is part of Spot, a model checking library.
//
// Spot is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// Spot is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "ltlast/atomic_prop.hh"
#include "defaultenv.hh"

namespace spot
{
  namespace ltl
  {

    default_environment::~default_environment()
    {
    }

    const formula*
    default_environment::require(const std::string& s)
    {
      return atomic_prop::instance(s, *this);
    }

    const std::string&
    default_environment::name()
    {
      static std::string name("default environment");
      return name;
    }

    default_environment::default_environment()
    {
    }

    default_environment&
    default_environment::instance()
    {
      static default_environment* singleton = new default_environment();
      return *singleton;
    }

  }
}