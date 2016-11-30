# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_buddy', [dirname(__file__)])
        except ImportError:
            import _buddy
            return _buddy
        if fp is not None:
            try:
                _mod = imp.load_module('_buddy', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _buddy = swig_import_helper()
    del swig_import_helper
else:
    import _buddy
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


class const_int_ptr(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, const_int_ptr, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, const_int_ptr, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _buddy.new_const_int_ptr(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_setmethods__["ptr"] = _buddy.const_int_ptr_ptr_set
    __swig_getmethods__["ptr"] = _buddy.const_int_ptr_ptr_get
    if _newclass:ptr = _swig_property(_buddy.const_int_ptr_ptr_get, _buddy.const_int_ptr_ptr_set)
    def __getitem__(self, *args): return _buddy.const_int_ptr___getitem__(self, *args)
    __swig_destroy__ = _buddy.delete_const_int_ptr
    __del__ = lambda self : None;
const_int_ptr_swigregister = _buddy.const_int_ptr_swigregister
const_int_ptr_swigregister(const_int_ptr)

class bdd(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, bdd, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, bdd, name)
    __repr__ = _swig_repr
    def id(self): return _buddy.bdd_id(self)
    def __cmp__(self, *args): return _buddy.bdd___cmp__(self, *args)
    def __le__(self, *args): return _buddy.bdd___le__(self, *args)
    def __lt__(self, *args): return _buddy.bdd___lt__(self, *args)
    def __eq__(self, *args): return _buddy.bdd___eq__(self, *args)
    def __ne__(self, *args): return _buddy.bdd___ne__(self, *args)
    def __ge__(self, *args): return _buddy.bdd___ge__(self, *args)
    def __gt__(self, *args): return _buddy.bdd___gt__(self, *args)
    def __str__(self, *args): return _buddy.bdd___str__(self, *args)
    def __and__(self, *args): return _buddy.bdd___and__(self, *args)
    def __xor__(self, *args): return _buddy.bdd___xor__(self, *args)
    def __or__(self, *args): return _buddy.bdd___or__(self, *args)
    def __rshift__(self, *args): return _buddy.bdd___rshift__(self, *args)
    def __lshift__(self, *args): return _buddy.bdd___lshift__(self, *args)
    def __sub__(self, *args): return _buddy.bdd___sub__(self, *args)
    def __neg__(self, *args): return _buddy.bdd___neg__(self, *args)
    def __init__(self): 
        this = _buddy.new_bdd()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _buddy.delete_bdd
    __del__ = lambda self : None;
bdd_swigregister = _buddy.bdd_swigregister
bdd_swigregister(bdd)


def bdd_init(*args):
  return _buddy.bdd_init(*args)
bdd_init = _buddy.bdd_init

def bdd_done():
  return _buddy.bdd_done()
bdd_done = _buddy.bdd_done

def bdd_setvarnum(*args):
  return _buddy.bdd_setvarnum(*args)
bdd_setvarnum = _buddy.bdd_setvarnum

def bdd_extvarnum(*args):
  return _buddy.bdd_extvarnum(*args)
bdd_extvarnum = _buddy.bdd_extvarnum

def bdd_isrunning():
  return _buddy.bdd_isrunning()
bdd_isrunning = _buddy.bdd_isrunning

def bdd_setmaxnodenum(*args):
  return _buddy.bdd_setmaxnodenum(*args)
bdd_setmaxnodenum = _buddy.bdd_setmaxnodenum

def bdd_setmaxincrease(*args):
  return _buddy.bdd_setmaxincrease(*args)
bdd_setmaxincrease = _buddy.bdd_setmaxincrease

def bdd_setminfreenodes(*args):
  return _buddy.bdd_setminfreenodes(*args)
bdd_setminfreenodes = _buddy.bdd_setminfreenodes

def bdd_getnodenum():
  return _buddy.bdd_getnodenum()
bdd_getnodenum = _buddy.bdd_getnodenum

def bdd_getallocnum():
  return _buddy.bdd_getallocnum()
bdd_getallocnum = _buddy.bdd_getallocnum

def bdd_versionstr():
  return _buddy.bdd_versionstr()
bdd_versionstr = _buddy.bdd_versionstr

def bdd_versionnum():
  return _buddy.bdd_versionnum()
bdd_versionnum = _buddy.bdd_versionnum

def bdd_fprintstat(*args):
  return _buddy.bdd_fprintstat(*args)
bdd_fprintstat = _buddy.bdd_fprintstat

def bdd_printstat():
  return _buddy.bdd_printstat()
bdd_printstat = _buddy.bdd_printstat

def bdd_errstring(*args):
  return _buddy.bdd_errstring(*args)
bdd_errstring = _buddy.bdd_errstring

def bdd_clear_error():
  return _buddy.bdd_clear_error()
bdd_clear_error = _buddy.bdd_clear_error

def bdd_ithvar(*args):
  return _buddy.bdd_ithvar(*args)
bdd_ithvar = _buddy.bdd_ithvar

def bdd_nithvar(*args):
  return _buddy.bdd_nithvar(*args)
bdd_nithvar = _buddy.bdd_nithvar

def bdd_var(*args):
  return _buddy.bdd_var(*args)
bdd_var = _buddy.bdd_var

def bdd_low(*args):
  return _buddy.bdd_low(*args)
bdd_low = _buddy.bdd_low

def bdd_high(*args):
  return _buddy.bdd_high(*args)
bdd_high = _buddy.bdd_high

def bdd_scanset(*args):
  return _buddy.bdd_scanset(*args)
bdd_scanset = _buddy.bdd_scanset

def bdd_makeset(*args):
  return _buddy.bdd_makeset(*args)
bdd_makeset = _buddy.bdd_makeset

def bdd_setbddpair(*args):
  return _buddy.bdd_setbddpair(*args)
bdd_setbddpair = _buddy.bdd_setbddpair

def bdd_replace(*args):
  return _buddy.bdd_replace(*args)
bdd_replace = _buddy.bdd_replace

def bdd_compose(*args):
  return _buddy.bdd_compose(*args)
bdd_compose = _buddy.bdd_compose

def bdd_veccompose(*args):
  return _buddy.bdd_veccompose(*args)
bdd_veccompose = _buddy.bdd_veccompose

def bdd_restrict(*args):
  return _buddy.bdd_restrict(*args)
bdd_restrict = _buddy.bdd_restrict

def bdd_constrain(*args):
  return _buddy.bdd_constrain(*args)
bdd_constrain = _buddy.bdd_constrain

def bdd_simplify(*args):
  return _buddy.bdd_simplify(*args)
bdd_simplify = _buddy.bdd_simplify

def bdd_ibuildcube(*args):
  return _buddy.bdd_ibuildcube(*args)
bdd_ibuildcube = _buddy.bdd_ibuildcube

def bdd_not(*args):
  return _buddy.bdd_not(*args)
bdd_not = _buddy.bdd_not

def bdd_apply(*args):
  return _buddy.bdd_apply(*args)
bdd_apply = _buddy.bdd_apply

def bdd_and(*args):
  return _buddy.bdd_and(*args)
bdd_and = _buddy.bdd_and

def bdd_or(*args):
  return _buddy.bdd_or(*args)
bdd_or = _buddy.bdd_or

def bdd_xor(*args):
  return _buddy.bdd_xor(*args)
bdd_xor = _buddy.bdd_xor

def bdd_imp(*args):
  return _buddy.bdd_imp(*args)
bdd_imp = _buddy.bdd_imp

def bdd_biimp(*args):
  return _buddy.bdd_biimp(*args)
bdd_biimp = _buddy.bdd_biimp

def bdd_setxor(*args):
  return _buddy.bdd_setxor(*args)
bdd_setxor = _buddy.bdd_setxor

def bdd_implies(*args):
  return _buddy.bdd_implies(*args)
bdd_implies = _buddy.bdd_implies

def bdd_ite(*args):
  return _buddy.bdd_ite(*args)
bdd_ite = _buddy.bdd_ite

def bdd_exist(*args):
  return _buddy.bdd_exist(*args)
bdd_exist = _buddy.bdd_exist

def bdd_existcomp(*args):
  return _buddy.bdd_existcomp(*args)
bdd_existcomp = _buddy.bdd_existcomp

def bdd_forall(*args):
  return _buddy.bdd_forall(*args)
bdd_forall = _buddy.bdd_forall

def bdd_forallcomp(*args):
  return _buddy.bdd_forallcomp(*args)
bdd_forallcomp = _buddy.bdd_forallcomp

def bdd_unique(*args):
  return _buddy.bdd_unique(*args)
bdd_unique = _buddy.bdd_unique

def bdd_uniquecomp(*args):
  return _buddy.bdd_uniquecomp(*args)
bdd_uniquecomp = _buddy.bdd_uniquecomp

def bdd_appex(*args):
  return _buddy.bdd_appex(*args)
bdd_appex = _buddy.bdd_appex

def bdd_appexcomp(*args):
  return _buddy.bdd_appexcomp(*args)
bdd_appexcomp = _buddy.bdd_appexcomp

def bdd_appall(*args):
  return _buddy.bdd_appall(*args)
bdd_appall = _buddy.bdd_appall

def bdd_appallcomp(*args):
  return _buddy.bdd_appallcomp(*args)
bdd_appallcomp = _buddy.bdd_appallcomp

def bdd_appuni(*args):
  return _buddy.bdd_appuni(*args)
bdd_appuni = _buddy.bdd_appuni

def bdd_appunicomp(*args):
  return _buddy.bdd_appunicomp(*args)
bdd_appunicomp = _buddy.bdd_appunicomp

def bdd_support(*args):
  return _buddy.bdd_support(*args)
bdd_support = _buddy.bdd_support

def bdd_satone(*args):
  return _buddy.bdd_satone(*args)
bdd_satone = _buddy.bdd_satone

def bdd_satoneset(*args):
  return _buddy.bdd_satoneset(*args)
bdd_satoneset = _buddy.bdd_satoneset

def bdd_fullsatone(*args):
  return _buddy.bdd_fullsatone(*args)
bdd_fullsatone = _buddy.bdd_fullsatone

def bdd_allsat(*args):
  return _buddy.bdd_allsat(*args)
bdd_allsat = _buddy.bdd_allsat

def bdd_satcount(*args):
  return _buddy.bdd_satcount(*args)
bdd_satcount = _buddy.bdd_satcount

def bdd_satcountset(*args):
  return _buddy.bdd_satcountset(*args)
bdd_satcountset = _buddy.bdd_satcountset

def bdd_satcountln(*args):
  return _buddy.bdd_satcountln(*args)
bdd_satcountln = _buddy.bdd_satcountln

def bdd_satcountlnset(*args):
  return _buddy.bdd_satcountlnset(*args)
bdd_satcountlnset = _buddy.bdd_satcountlnset

def bdd_nodecount(*args):
  return _buddy.bdd_nodecount(*args)
bdd_nodecount = _buddy.bdd_nodecount

def bdd_varprofile(*args):
  return _buddy.bdd_varprofile(*args)
bdd_varprofile = _buddy.bdd_varprofile

def bdd_pathcount(*args):
  return _buddy.bdd_pathcount(*args)
bdd_pathcount = _buddy.bdd_pathcount

def bdd_fprinttable(*args):
  return _buddy.bdd_fprinttable(*args)
bdd_fprinttable = _buddy.bdd_fprinttable

def bdd_printtable(*args):
  return _buddy.bdd_printtable(*args)
bdd_printtable = _buddy.bdd_printtable

def bdd_fprintset(*args):
  return _buddy.bdd_fprintset(*args)
bdd_fprintset = _buddy.bdd_fprintset

def bdd_printset(*args):
  return _buddy.bdd_printset(*args)
bdd_printset = _buddy.bdd_printset

def bdd_printdot(*args):
  return _buddy.bdd_printdot(*args)
bdd_printdot = _buddy.bdd_printdot

def bdd_fprintdot(*args):
  return _buddy.bdd_fprintdot(*args)
bdd_fprintdot = _buddy.bdd_fprintdot

def bdd_fnprintdot(*args):
  return _buddy.bdd_fnprintdot(*args)
bdd_fnprintdot = _buddy.bdd_fnprintdot

def bdd_fnsave(*args):
  return _buddy.bdd_fnsave(*args)
bdd_fnsave = _buddy.bdd_fnsave

def bdd_save(*args):
  return _buddy.bdd_save(*args)
bdd_save = _buddy.bdd_save

def bdd_fnload(*args):
  return _buddy.bdd_fnload(*args)
bdd_fnload = _buddy.bdd_fnload

def bdd_load(*args):
  return _buddy.bdd_load(*args)
bdd_load = _buddy.bdd_load

def bdd_addvarblock(*args):
  return _buddy.bdd_addvarblock(*args)
bdd_addvarblock = _buddy.bdd_addvarblock
bddop_and = _buddy.bddop_and
bddop_xor = _buddy.bddop_xor
bddop_or = _buddy.bddop_or
bddop_nand = _buddy.bddop_nand
bddop_nor = _buddy.bddop_nor
bddop_imp = _buddy.bddop_imp
bddop_biimp = _buddy.bddop_biimp
bddop_diff = _buddy.bddop_diff
bddop_less = _buddy.bddop_less
bddop_invimp = _buddy.bddop_invimp
BDD_REORDER_NONE = _buddy.BDD_REORDER_NONE
BDD_REORDER_WIN2 = _buddy.BDD_REORDER_WIN2
BDD_REORDER_WIN2ITE = _buddy.BDD_REORDER_WIN2ITE
BDD_REORDER_SIFT = _buddy.BDD_REORDER_SIFT
BDD_REORDER_SIFTITE = _buddy.BDD_REORDER_SIFTITE
BDD_REORDER_WIN3 = _buddy.BDD_REORDER_WIN3
BDD_REORDER_WIN3ITE = _buddy.BDD_REORDER_WIN3ITE
BDD_REORDER_RANDOM = _buddy.BDD_REORDER_RANDOM

def fdd_extdomain(*args):
  return _buddy.fdd_extdomain(*args)
fdd_extdomain = _buddy.fdd_extdomain

def fdd_overlapdomain(*args):
  return _buddy.fdd_overlapdomain(*args)
fdd_overlapdomain = _buddy.fdd_overlapdomain

def fdd_clearall():
  return _buddy.fdd_clearall()
fdd_clearall = _buddy.fdd_clearall

def fdd_domainnum():
  return _buddy.fdd_domainnum()
fdd_domainnum = _buddy.fdd_domainnum

def fdd_domainsize(*args):
  return _buddy.fdd_domainsize(*args)
fdd_domainsize = _buddy.fdd_domainsize

def fdd_varnum(*args):
  return _buddy.fdd_varnum(*args)
fdd_varnum = _buddy.fdd_varnum

def fdd_vars(*args):
  return _buddy.fdd_vars(*args)
fdd_vars = _buddy.fdd_vars

def fdd_ithvar(*args):
  return _buddy.fdd_ithvar(*args)
fdd_ithvar = _buddy.fdd_ithvar

def fdd_scanvar(*args):
  return _buddy.fdd_scanvar(*args)
fdd_scanvar = _buddy.fdd_scanvar

def fdd_scanallvar(*args):
  return _buddy.fdd_scanallvar(*args)
fdd_scanallvar = _buddy.fdd_scanallvar

def fdd_ithset(*args):
  return _buddy.fdd_ithset(*args)
fdd_ithset = _buddy.fdd_ithset

def fdd_domain(*args):
  return _buddy.fdd_domain(*args)
fdd_domain = _buddy.fdd_domain

def fdd_equals(*args):
  return _buddy.fdd_equals(*args)
fdd_equals = _buddy.fdd_equals

def fdd_printset(*args):
  return _buddy.fdd_printset(*args)
fdd_printset = _buddy.fdd_printset

def fdd_fprintset(*args):
  return _buddy.fdd_fprintset(*args)
fdd_fprintset = _buddy.fdd_fprintset

def fdd_scanset(*args):
  return _buddy.fdd_scanset(*args)
fdd_scanset = _buddy.fdd_scanset

def fdd_makeset(*args):
  return _buddy.fdd_makeset(*args)
fdd_makeset = _buddy.fdd_makeset

def fdd_intaddvarblock(*args):
  return _buddy.fdd_intaddvarblock(*args)
fdd_intaddvarblock = _buddy.fdd_intaddvarblock

def fdd_setpair(*args):
  return _buddy.fdd_setpair(*args)
fdd_setpair = _buddy.fdd_setpair

def fdd_setpairs(*args):
  return _buddy.fdd_setpairs(*args)
fdd_setpairs = _buddy.fdd_setpairs

def bvec_copy(*args):
  return _buddy.bvec_copy(*args)
bvec_copy = _buddy.bvec_copy

def bvec_true(*args):
  return _buddy.bvec_true(*args)
bvec_true = _buddy.bvec_true

def bvec_false(*args):
  return _buddy.bvec_false(*args)
bvec_false = _buddy.bvec_false

def bvec_con(*args):
  return _buddy.bvec_con(*args)
bvec_con = _buddy.bvec_con

def bvec_var(*args):
  return _buddy.bvec_var(*args)
bvec_var = _buddy.bvec_var

def bvec_varfdd(*args):
  return _buddy.bvec_varfdd(*args)
bvec_varfdd = _buddy.bvec_varfdd

def bvec_varvec(*args):
  return _buddy.bvec_varvec(*args)
bvec_varvec = _buddy.bvec_varvec

def bvec_coerce(*args):
  return _buddy.bvec_coerce(*args)
bvec_coerce = _buddy.bvec_coerce

def bvec_isconst(*args):
  return _buddy.bvec_isconst(*args)
bvec_isconst = _buddy.bvec_isconst

def bvec_val(*args):
  return _buddy.bvec_val(*args)
bvec_val = _buddy.bvec_val

def bvec_map1(*args):
  return _buddy.bvec_map1(*args)
bvec_map1 = _buddy.bvec_map1

def bvec_map2(*args):
  return _buddy.bvec_map2(*args)
bvec_map2 = _buddy.bvec_map2

def bvec_map3(*args):
  return _buddy.bvec_map3(*args)
bvec_map3 = _buddy.bvec_map3

def bvec_add(*args):
  return _buddy.bvec_add(*args)
bvec_add = _buddy.bvec_add

def bvec_sub(*args):
  return _buddy.bvec_sub(*args)
bvec_sub = _buddy.bvec_sub

def bvec_mulfixed(*args):
  return _buddy.bvec_mulfixed(*args)
bvec_mulfixed = _buddy.bvec_mulfixed

def bvec_mul(*args):
  return _buddy.bvec_mul(*args)
bvec_mul = _buddy.bvec_mul

def bvec_divfixed(*args):
  return _buddy.bvec_divfixed(*args)
bvec_divfixed = _buddy.bvec_divfixed

def bvec_div(*args):
  return _buddy.bvec_div(*args)
bvec_div = _buddy.bvec_div

def bvec_ite(*args):
  return _buddy.bvec_ite(*args)
bvec_ite = _buddy.bvec_ite

def bvec_shlfixed(*args):
  return _buddy.bvec_shlfixed(*args)
bvec_shlfixed = _buddy.bvec_shlfixed

def bvec_shl(*args):
  return _buddy.bvec_shl(*args)
bvec_shl = _buddy.bvec_shl

def bvec_shrfixed(*args):
  return _buddy.bvec_shrfixed(*args)
bvec_shrfixed = _buddy.bvec_shrfixed

def bvec_shr(*args):
  return _buddy.bvec_shr(*args)
bvec_shr = _buddy.bvec_shr

def bvec_lth(*args):
  return _buddy.bvec_lth(*args)
bvec_lth = _buddy.bvec_lth

def bvec_lte(*args):
  return _buddy.bvec_lte(*args)
bvec_lte = _buddy.bvec_lte

def bvec_gth(*args):
  return _buddy.bvec_gth(*args)
bvec_gth = _buddy.bvec_gth

def bvec_gte(*args):
  return _buddy.bvec_gte(*args)
bvec_gte = _buddy.bvec_gte

def bvec_equ(*args):
  return _buddy.bvec_equ(*args)
bvec_equ = _buddy.bvec_equ

def bvec_neq(*args):
  return _buddy.bvec_neq(*args)
bvec_neq = _buddy.bvec_neq
class bvec(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, bvec, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, bvec, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _buddy.new_bvec(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _buddy.delete_bvec
    __del__ = lambda self : None;
    def set(self, *args): return _buddy.bvec_set(self, *args)
    def bitnum(self): return _buddy.bvec_bitnum(self)
    def empty(self): return _buddy.bvec_empty(self)
    def __and__(self, *args): return _buddy.bvec___and__(self, *args)
    def __xor__(self, *args): return _buddy.bvec___xor__(self, *args)
    def __or__(self, *args): return _buddy.bvec___or__(self, *args)
    def __lshift__(self, *args): return _buddy.bvec___lshift__(self, *args)
    def __rshift__(self, *args): return _buddy.bvec___rshift__(self, *args)
    def __add__(self, *args): return _buddy.bvec___add__(self, *args)
    def __sub__(self, *args): return _buddy.bvec___sub__(self, *args)
    def __mul__(self, *args): return _buddy.bvec___mul__(self, *args)
    def __lt__(self, *args): return _buddy.bvec___lt__(self, *args)
    def __le__(self, *args): return _buddy.bvec___le__(self, *args)
    def __gt__(self, *args): return _buddy.bvec___gt__(self, *args)
    def __ge__(self, *args): return _buddy.bvec___ge__(self, *args)
    def __eq__(self, *args): return _buddy.bvec___eq__(self, *args)
    def __ne__(self, *args): return _buddy.bvec___ne__(self, *args)
    def __str__(self, *args): return _buddy.bvec___str__(self, *args)
    def __getitem__(self, *args): return _buddy.bvec___getitem__(self, *args)
bvec_swigregister = _buddy.bvec_swigregister
bvec_swigregister(bvec)
cvar = _buddy.cvar
bddfalse = cvar.bddfalse
bddtrue = cvar.bddtrue

# This file is compatible with both classic and new-style classes.

