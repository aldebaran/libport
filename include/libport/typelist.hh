/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_META_TYPELIST_HH
# define LIBPORT_META_TYPELIST_HH

// Avoid numerous unrelevant warnings triggered by metaprogramming
# if defined __GNUC__
#  pragma GCC system_header
# endif

# include <libport/meta.hh>
# include <libport/preproc.hh>

// Ruby to generate the following macros
//
// 51.times {|i|
//   print "\#define TYPELIST_#{i}("
//   (i-1).times {|j|
//     print "T#{j + 1}, "
//   }
//   print "T#{i}) typelist::List<T1, TYPELIST_#{i - 1}("
//   (i-2).times {|j|
//     print "T#{j + 2}, "
//   }
//   puts "T#{i})>"
// }

#define TYPELIST_0() libport::meta::typelist::Null
#define TYPELIST_1(T1) libport::meta::typelist::List<T1, TYPELIST_0()>
#define TYPELIST_2(T1, T2) libport::meta::typelist::List<T1, TYPELIST_1(T2)>
#define TYPELIST_3(T1, T2, T3) libport::meta::typelist::List<T1, TYPELIST_2(T2, T3)>
#define TYPELIST_4(T1, T2, T3, T4) libport::meta::typelist::List<T1, TYPELIST_3(T2, T3, T4)>
#define TYPELIST_5(T1, T2, T3, T4, T5) libport::meta::typelist::List<T1, TYPELIST_4(T2, T3, T4, T5)>
#define TYPELIST_6(T1, T2, T3, T4, T5, T6) libport::meta::typelist::List<T1, TYPELIST_5(T2, T3, T4, T5, T6)>
#define TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) libport::meta::typelist::List<T1, TYPELIST_6(T2, T3, T4, T5, T6, T7)>
#define TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) libport::meta::typelist::List<T1, TYPELIST_7(T2, T3, T4, T5, T6, T7, T8)>
#define TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) libport::meta::typelist::List<T1, TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9)>
#define TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) libport::meta::typelist::List<T1, TYPELIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10)>
#define TYPELIST_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) libport::meta::typelist::List<T1, TYPELIST_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>
#define TYPELIST_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) libport::meta::typelist::List<T1, TYPELIST_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>
#define TYPELIST_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) libport::meta::typelist::List<T1, TYPELIST_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>
#define TYPELIST_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) libport::meta::typelist::List<T1, TYPELIST_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>
#define TYPELIST_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) libport::meta::typelist::List<T1, TYPELIST_14(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>
#define TYPELIST_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16) libport::meta::typelist::List<T1, TYPELIST_15(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16)>
#define TYPELIST_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17) libport::meta::typelist::List<T1, TYPELIST_16(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17)>
#define TYPELIST_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18) libport::meta::typelist::List<T1, TYPELIST_17(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18)>
#define TYPELIST_19(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19) libport::meta::typelist::List<T1, TYPELIST_18(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19)>
#define TYPELIST_20(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20) libport::meta::typelist::List<T1, TYPELIST_19(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20)>
#define TYPELIST_21(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21) libport::meta::typelist::List<T1, TYPELIST_20(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21)>
#define TYPELIST_22(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22) libport::meta::typelist::List<T1, TYPELIST_21(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22)>
#define TYPELIST_23(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23) libport::meta::typelist::List<T1, TYPELIST_22(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23)>
#define TYPELIST_24(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24) libport::meta::typelist::List<T1, TYPELIST_23(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24)>
#define TYPELIST_25(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25) libport::meta::typelist::List<T1, TYPELIST_24(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25)>
#define TYPELIST_26(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26) libport::meta::typelist::List<T1, TYPELIST_25(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26)>
#define TYPELIST_27(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27) libport::meta::typelist::List<T1, TYPELIST_26(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27)>
#define TYPELIST_28(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28) libport::meta::typelist::List<T1, TYPELIST_27(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28)>
#define TYPELIST_29(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29) libport::meta::typelist::List<T1, TYPELIST_28(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29)>
#define TYPELIST_30(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30) libport::meta::typelist::List<T1, TYPELIST_29(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30)>
#define TYPELIST_31(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31) libport::meta::typelist::List<T1, TYPELIST_30(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31)>
#define TYPELIST_32(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32) libport::meta::typelist::List<T1, TYPELIST_31(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32)>
#define TYPELIST_33(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33) libport::meta::typelist::List<T1, TYPELIST_32(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33)>
#define TYPELIST_34(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34) libport::meta::typelist::List<T1, TYPELIST_33(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34)>
#define TYPELIST_35(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35) libport::meta::typelist::List<T1, TYPELIST_34(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35)>
#define TYPELIST_36(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36) libport::meta::typelist::List<T1, TYPELIST_35(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36)>
#define TYPELIST_37(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37) libport::meta::typelist::List<T1, TYPELIST_36(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37)>
#define TYPELIST_38(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38) libport::meta::typelist::List<T1, TYPELIST_37(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38)>
#define TYPELIST_39(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39) libport::meta::typelist::List<T1, TYPELIST_38(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39)>
#define TYPELIST_40(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40) libport::meta::typelist::List<T1, TYPELIST_39(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40)>
#define TYPELIST_41(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41) libport::meta::typelist::List<T1, TYPELIST_40(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41)>
#define TYPELIST_42(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42) libport::meta::typelist::List<T1, TYPELIST_41(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42)>
#define TYPELIST_43(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43) libport::meta::typelist::List<T1, TYPELIST_42(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43)>
#define TYPELIST_44(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44) libport::meta::typelist::List<T1, TYPELIST_43(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44)>
#define TYPELIST_45(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45) libport::meta::typelist::List<T1, TYPELIST_44(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45)>
#define TYPELIST_46(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46) libport::meta::typelist::List<T1, TYPELIST_45(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46)>
#define TYPELIST_47(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47) libport::meta::typelist::List<T1, TYPELIST_46(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47)>
#define TYPELIST_48(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48) libport::meta::typelist::List<T1, TYPELIST_47(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48)>
#define TYPELIST_49(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49) libport::meta::typelist::List<T1, TYPELIST_48(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49)>
#define TYPELIST_50(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50) libport::meta::typelist::List<T1, TYPELIST_49(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50)>
#define TYPELIST_51(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51) libport::meta::typelist::List<T1, TYPELIST_50(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51)>
#define TYPELIST_52(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52) libport::meta::typelist::List<T1, TYPELIST_51(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52)>

// BOOST_PP_CAT concatenates the wrong way under vcxx.
// ..so does LIBPORT_CAT, but its a different failure
#define TYPELIST_ITEM(_, __, t) \
  libport::meta::typelist::List<t,
#define TYPELIST_ITEMEND(_, __, t) > /**/

#define TYPELIST(...)                                          \
  LIBPORT_VAARGS_APPLY(TYPELIST_ITEM, _, __VA_ARGS__)          \
  libport::meta::typelist::Null                                \
  LIBPORT_VAARGS_APPLY(TYPELIST_ITEMEND, _, __VA_ARGS__)

//#define TYPELIST(...) LIBPORT_CAT(TYPELIST_, LIBPORT_LIST_VASIZE((__VA_ARGS__,)))(__VA_ARGS__)

namespace libport
{
  namespace meta
  {
    namespace typelist
    {
      // Fwd
      template <typename T>
      struct IsList;

      // Errors
      template <typename T>
      struct Is_not_a_list {};

      template <typename T>
      struct Not_in_list {};

      template <typename T>
      struct Out_of_bound {};

      // AssertList
      template <typename T>
      struct AssertList
        : public Assert<IsList<T>::res, Is_not_a_list<T> >
      {};

      // Data types
      struct Null {};

      template <typename H, typename T>
      struct List: public AssertList<T>
      {
          typedef H Head;
          typedef T Tail;
      };

      // IsList
      template <typename T>
      struct IsList
      {
          static const bool res = false;
      };

      template <>
      struct IsList<Null>
      {
        static const bool res = true;
      };

      template <typename H, typename T>
      struct IsList<List<H, T> >
      {
          static const bool res = true;
      };

      // Length
      template <typename T>
      struct Length: public AssertList<T>
      {
          static const int res = 0;
      };

      template <typename H, typename T>
      struct Length<List<H, T> >
      {
          static const int res = 1 + Length<T>::res;
      };

      // At
      template <typename T, int I>
      struct At:
          private Uniquify <Assert <I >= 0, Must_be_positive<I> >, 1>,
          private Error<Out_of_bound<T> >
      {
          typedef Null res;
      };

      template <typename H, typename T>
      struct At<List<H, T>, 0>
      {
          typedef H res;
      };

      template <typename H, typename T, int I>
      struct At<List<H, T>, I>:
          private Assert <I >= 0, Must_be_positive<I> >
      {
          typedef typename At<T, I - 1>::res res;
      };

      /// LooseAt: same as At, but return Null if out of bound
      template <typename T, int I>
      struct LooseAt:
        private Assert <I >= 0, Must_be_positive<I> >
      {
        typedef Null res;
      };

      template <typename H, typename T>
      struct LooseAt<List<H, T>, 0>
      {
        typedef H res;
      };

      template <typename H, typename T, int I>
      struct LooseAt<List<H, T>, I>
        : private Assert <I >= 0, Must_be_positive<I> >
      {
        typedef typename LooseAt<T, I - 1>::res res;
      };

      // Contains
      template <typename L, typename V>
      struct Contains
        : private Assert<IsList<L>::res, Is_not_a_list<V> >
      {
        static const bool res = false;
      };

      template <typename H, typename T, typename V>
      struct Contains<List<H, T>, V>
      {
        static const bool res = Contains<T, V>::res;
      };

      template <typename T, typename V>
      struct Contains<List<V, T>, V>
      {
        static const bool res = true;
      };

      // Idx
      template <typename L, typename V, int I>
      struct IdxRec
        : private Assert<IsList<L>::res, Is_not_a_list<V> >
        , private Assert<false, Not_in_list<V> >
      {};

      template <typename H, typename T, typename V, int I>
      struct IdxRec<List<H, T>, V, I>
      {
        static const int res = IdxRec<T, V, I + 1>::res;
      };

      template <typename T, typename V, int I>
      struct IdxRec<List<V, T>, V, I>
      {
        static const int res = I;
      };

      template <typename L, typename V>
      struct Idx
      {
        static const int res = IdxRec<L, V, 0>::res;
      };

      // Max.
      template <typename L, template <typename, typename> class Comp>
      struct Max: private Assert<IsList<L>::res, Is_not_a_list<L> >::res
      {
        typedef Null res;
      };


      template <typename H, typename T,
                template <typename, typename> class Comp>
      struct Max<List<H, T>, Comp>
      {
        typedef typename Max<T, Comp>::res next;
        typedef typename If<Comp<H, next>::res || Eq<next, Null>::res,
                            H, next>::res res;
      };

      // Foreach.
      template <typename C, typename L, template<typename> class F>
      struct ForEach
      {
        void operator()(C&)
        {
        }
      };
      template <typename C, typename H, typename T,
                template<typename> class F>
      struct ForEach<C, List<H, T>, F>
      {
        void operator()(C& ctx)
        {
          F<H>()(ctx);
          ForEach<C, T, F>()(ctx);
        }
      };

      // Map
      template <template <class> class F, typename L>
      struct Map: private Assert<IsList<L>::res, Is_not_a_list<L> >::res
      {
        typedef Null res;
      };

      template <template <class> class F, typename H, typename T>
      struct Map<F, List<H, T> >
      {
        typedef List<F<H>, typename Map<F, T>::res> res;
      };

      // MapBind
      template <template <class, class> class F, typename L, typename V>
      struct MapBind: private Assert<IsList<L>::res, Is_not_a_list<L> >::res
      {
        typedef Null res;
      };

      template <template <class, class> class F,
                typename H, typename T, typename V>
      struct MapBind<F, List<H, T>, V>
      {
          typedef List<F<H, V>, typename MapBind<F, T, V>::res> res;
      };

      // Collapse
      template <typename L>
      struct Collapse: private Assert<IsList<L>::res, Is_not_a_list<L> >::res
      {};

      template <typename H, typename T>
      struct Collapse<List<H, T> >: public H, public Collapse<T>
      {};

      // Add
      template <typename L, typename Elt>
      struct Add
        : AssertList<L>
      {
          typedef TYPELIST_1(Elt) res;
      };

      template <typename H, typename T, typename Elt>
      struct Add<List<H, T>, Elt>
      {
          typedef List<H, typename Add<T, Elt>::res> res;
      };

      // Fold
      template <template <typename, typename> class F, typename L>
      struct Fold
      {};

      template <template <typename, typename> class F, typename H>
      struct Fold<F, meta::typelist::List<H, meta::typelist::Null> >
      {
        typedef H res;
      };

      template <template <typename, typename> class F, typename H, typename T>
      struct Fold<F, meta::typelist::List<H, T> >
      {
        typedef F<H, typename Fold<F, T>::res> res;
      };
    }
  }
}

#endif
