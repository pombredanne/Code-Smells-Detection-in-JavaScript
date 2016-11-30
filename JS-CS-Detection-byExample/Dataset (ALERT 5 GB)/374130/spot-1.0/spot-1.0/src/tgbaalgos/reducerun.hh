// Copyright (C) 2010 Laboratoire de Recherche et Developpement de
// l'Epita.
// Copyright (C) 2004  Laboratoire d'Informatique de Paris 6 (LIP6),
// d�partement Syst�mes R�partis Coop�ratifs (SRC), Universit� Pierre
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

#ifndef SPOT_TGBAALGOS_REDUCERUN_HH
# define SPOT_TGBAALGOS_REDUCERUN_HH

namespace spot
{
  class tgba;
  struct tgba_run;

  /// \brief Reduce an accepting run.
  /// \ingroup tgba_run
  ///
  /// Return a run which is accepting for \a and that is no longer
  /// that \a org.
  tgba_run* reduce_run(const tgba* a, const tgba_run* org);
}

#endif // SPOT_TGBAALGOS_REDUCERUN_HH