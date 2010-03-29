/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIPBORT_HIERARCHY_HH
# define LIPBORT_HIERARCHY_HH

# include <map>

# include <libport/cassert>
# include <libport/containers.hh>
# include <libport/meta.hh>
# include <libport/type-info.hh>
# include <libport/typelist.hh>

#define ENUMERATE(Macro)                        \
  Macro(0);                                     \
  Macro(1);                                     \
  Macro(2);                                     \
  Macro(3);                                     \
  Macro(4);                                     \
  Macro(5);                                     \
  Macro(6);                                     \
  Macro(7);                                     \
  Macro(8);                                     \
  Macro(9);                                     \
  Macro(10);                                    \
  Macro(11);                                    \
  Macro(12);                                    \
  Macro(13);                                    \
  Macro(14);                                    \
  Macro(15);                                    \
  Macro(16);                                    \
  Macro(17);                                    \
  Macro(18);                                    \
  Macro(19);                                    \
  Macro(20);                                    \
  Macro(21);                                    \
  Macro(22);                                    \
  Macro(23);                                    \
  Macro(24);                                    \
  Macro(25);                                    \
  Macro(26);                                    \
  Macro(27);                                    \
  Macro(28);                                    \
  Macro(29);                                    \
  Macro(30);                                    \
  Macro(31);                                    \
  Macro(32);                                    \
  Macro(33);                                    \
  Macro(34);                                    \
  Macro(35);                                    \
  Macro(36);                                    \
  Macro(37);                                    \
  Macro(38);                                    \
  Macro(39);                                    \
  Macro(40);                                    \
  Macro(41);                                    \
  Macro(42);                                    \
  Macro(43);                                    \
  Macro(44);                                    \
  Macro(45);                                    \
  Macro(46);                                    \
  Macro(47);                                    \
  Macro(48);                                    \
  Macro(49);                                    \
  Macro(50);                                    \
  Macro(51);                                    \
  Macro(52);                                    \
  Macro(53);                                    \
  Macro(54);                                    \
  Macro(55);                                    \
  Macro(56);                                    \
  Macro(57);                                    \
  Macro(58);                                    \
  Macro(59);                                    \
  Macro(60);                                    \
  Macro(61);                                    \
  Macro(62);                                    \
  Macro(63);                                    \
  Macro(64);                                    \
  Macro(65);                                    \
  Macro(66);                                    \
  Macro(67);                                    \
  Macro(68);                                    \
  Macro(69);                                    \
  Macro(70);                                    \
  Macro(71);                                    \
  Macro(72);                                    \
  Macro(73);                                    \
  Macro(74);                                    \
  Macro(75);                                    \
  Macro(76);                                    \
  Macro(77);                                    \
  Macro(78);                                    \
  Macro(79);                                    \
  Macro(80);                                    \
  Macro(81);                                    \
  Macro(82);                                    \
  Macro(83);                                    \
  Macro(84);                                    \
  Macro(85);                                    \
  Macro(86);                                    \
  Macro(87);                                    \
  Macro(88);                                    \
  Macro(89);                                    \
  Macro(90);                                    \
  Macro(91);                                    \
  Macro(92);                                    \
  Macro(93);                                    \
  Macro(94);                                    \
  Macro(95);                                    \
  Macro(96);                                    \
  Macro(97);                                    \
  Macro(98);                                    \
  Macro(99);                                    \
  Macro(100);                                   \
  Macro(101);                                   \
  Macro(102);                                   \
  Macro(103);                                   \
  Macro(104);                                   \
  Macro(105);                                   \
  Macro(106);                                   \
  Macro(107);                                   \
  Macro(108);                                   \
  Macro(109);                                   \
  Macro(110);                                   \
  Macro(111);                                   \
  Macro(112);                                   \
  Macro(113);                                   \
  Macro(114);                                   \
  Macro(115);                                   \
  Macro(116);                                   \
  Macro(117);                                   \
  Macro(118);                                   \
  Macro(119);                                   \
  Macro(120);                                   \
  Macro(121);                                   \
  Macro(122);                                   \
  Macro(123);                                   \
  Macro(124);                                   \
  Macro(125);                                   \
  Macro(126);                                   \
  Macro(127);                                   \
  Macro(128);


namespace libport
{
  namespace meta
  {
    template <typename Data>
    struct Noop
    {
      static inline void res(Data&) {}
    };

    struct BaseHierarchy
    {
      virtual ~BaseHierarchy()
      {}
    };

    template <typename Root, typename Types>
    class Hierarchy: public BaseHierarchy
    {
    public:
      typedef unsigned Id;
      typedef Root root;
      typedef Types types;

      template <template <typename> class Impl, typename Data>
      static inline void
      dispatch(unsigned id, Data& d)
      {
        switch (id)
        {
#define CASE(N)                                                         \
          case N:                                                       \
            {                                                           \
              static const bool test =                                  \
                N < typelist::Length<Types>::res;                       \
              typedef typename typelist::LooseAt<Types, N>::res Type;   \
              typedef                                                   \
                typename If<test, Impl<Type>, Noop<Data> >::res         \
                Res;                                                    \
              Res::res(d);                                              \
              break;                                                    \
            }                                                           \

          ENUMERATE(CASE);
#undef CASE
        }
      }

      static Id
      id(const Root* r)
      {
        TypeInfo i(*r);
        aver(mhas(ids_(), i));
        return ids_().find(i)->second;
      }

      static inline Id
      id(const Root& r)
      {
        return id(&r);
      }

      inline Id
      id() const
      {
        return id(static_cast<const Root*>(this));
      }

    private:
      static inline const std::map<TypeInfo, Id>& ids_()
      {
        static bool initialized = false;
        static std::map<TypeInfo, Id> res;
        if (!initialized)
        {
          initialized = true;
#define REGISTER(N) res[typeid(typename typelist::LooseAt<Types, N>::res)] = N;
          ENUMERATE(REGISTER);
#undef REGISTER
        }
        return res;
      }
    };
  }
}

#endif
