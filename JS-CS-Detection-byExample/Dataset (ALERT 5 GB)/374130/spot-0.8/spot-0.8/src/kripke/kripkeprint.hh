// Copyright (C) 2011 Laboratoire de Recherche et Developpement
// de l'Epita (LRDE)
//
// This file is part of Spot, a model checking library.
//
// Spot is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Spot is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Spot; see the file COPYING.  If not, write to the Free
// Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.


#ifndef SPOT_KRIPKE_KRIPKEPRINT_HH
# define SPOT_KRIPKE_KRIPKEPRINT_HH

# include <iosfwd>
namespace spot
{

  class kripke;

  /// \brief Save the reachable part of Kripke structure in text format.
  ///
  /// The states will be named with the value returned by the
  /// kripke::format_state() method.  Such a string can be large, so
  /// the output will not be I/O efficient.  We recommend using this
  /// function only for debugging.  Use
  /// kripke_save_reachable_renumbered() for large output.
  ///
  /// \ingroup tgba_io
  std::ostream& kripke_save_reachable(std::ostream& os, const kripke* k);

  /// \brief Save the reachable part of Kripke structure in text format.
  ///
  /// States will be renumbered with sequential number.  This is much
  /// more I/O efficient when dumping large Kripke structures with big
  /// state names.  The drawback is that any information carried by
  /// the state name is lost.
  ///
  /// \ingroup tgba_io
  std::ostream& kripke_save_reachable_renumbered(std::ostream& os,
						 const kripke* k);

} // End namespace spot

#endif /* !KRIPKEPRINT_HH_ */