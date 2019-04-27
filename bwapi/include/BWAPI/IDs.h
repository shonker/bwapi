#pragma once

#include <vector>

namespace BWAPI {
  struct NoneIdentifier {};

  template<typename T>
  struct Identifier {
    int id;
    using IdT = Identifier;
    constexpr explicit Identifier(int id_) : id{id_} { }
    constexpr Identifier(NoneIdentifier) : id{ -1 } { }
    T getID() const { return T{id}; }
    constexpr explicit operator bool() const { return id >= 0; }
    constexpr explicit operator int() const  { return id; }
    constexpr bool operator==(Identifier const &other) const { return id == other.id; }
    constexpr bool operator!=(Identifier const &other) const { return id != other.id; }
    constexpr bool operator< (Identifier const &other) const { return id <  other.id; }
    constexpr bool operator> (Identifier const &other) const { return id >  other.id; }
    constexpr bool operator<=(Identifier const &other) const { return id <= other.id; }
    constexpr bool operator>=(Identifier const &other) const { return id >= other.id; }
    constexpr bool operator!=(std::nullptr_t) const { return static_cast<bool>(*this); }
    constexpr bool operator==(std::nullptr_t) const { return !(*this != nullptr); }

    struct Hash {
      auto operator()(Identifier id) {
        return std::hash<int>{}(static_cast<int>(id.getID()));
      }
    };

    using list = std::vector<T>;

    static constexpr NoneIdentifier None{};
  };

  struct UnitID   : Identifier<UnitID>   { using Identifier<UnitID>  ::Identifier; };
  struct PlayerID : Identifier<PlayerID> { using Identifier<PlayerID>::Identifier; };
  struct RegionID : Identifier<RegionID> { using Identifier<RegionID>::Identifier; };
  struct BulletID : Identifier<BulletID> { using Identifier<BulletID>::Identifier; };
  struct ForceID  : Identifier<ForceID>  { using Identifier<ForceID> ::Identifier; };
} // namespace BWAPI

namespace std {
  template<typename T>
  struct hash<BWAPI::Identifier<T>> {
    auto operator()(BWAPI::Identifier<T> id) const {
      return BWAPI::Identifier<T>::Hash{}(id);
    }
  };
} // namespace std

namespace BWAPI {
  class Game;

  template<typename Interface, typename Data>
  struct InterfaceDataWrapper {
    Data const *dataptr;
    using IdT = std::decay_t<decltype(Data::id)>;
    constexpr InterfaceDataWrapper(Data const &ptr) : dataptr{ &ptr } { }
    constexpr InterfaceDataWrapper(std::nullptr_t) : dataptr{ nullptr } { }
    constexpr auto const &getData() const { return *dataptr; }
    constexpr Game &getGame() const { return dataptr->game; }
    constexpr explicit operator bool() const { return dataptr; }
    constexpr explicit operator int() const { return static_cast<int>(dataptr->id); }
    constexpr bool operator==(InterfaceDataWrapper const &other) const { return dataptr->id == other.dataptr->id; }
    constexpr bool operator!=(InterfaceDataWrapper const &other) const { return dataptr->id != other.dataptr->id; }
    constexpr bool operator< (InterfaceDataWrapper const &other) const { return dataptr->id <  other.dataptr->id; }
    constexpr bool operator> (InterfaceDataWrapper const &other) const { return dataptr->id >  other.dataptr->id; }
    constexpr bool operator<=(InterfaceDataWrapper const &other) const { return dataptr->id <= other.dataptr->id; }
    constexpr bool operator>=(InterfaceDataWrapper const &other) const { return dataptr->id >= other.dataptr->id; }
    constexpr bool operator!=(std::nullptr_t) const { return dataptr != nullptr; }
    constexpr bool operator==(std::nullptr_t) const { return dataptr == nullptr; }

    struct Hash { 
      auto operator()(InterfaceDataWrapper &idw) {
        return decltype(Data::id)::Hash{}(idw.getData().id);
      }
    };

    // Backwards compatibility with when BWAPI::Unit etc were pointers
    constexpr Interface *operator->() { return static_cast<Interface *>(this); }
    constexpr Interface const *operator->() const { return static_cast<Interface const *>(this); }
    constexpr explicit operator IdT() const { return getData().id; }
  };

  struct IDCompare {
    using is_transparent = std::true_type;
    template<typename Lhs, typename Rhs>
    auto operator()(Lhs const &lhs, Rhs const &rhs) const
    {
      return Lhs::IdT{ lhs } < Rhs::IdT{ rhs };
    }
    template<typename T>
    auto operator()(T const &lhs, T const &rhs) const
    {
      return IDCompare{}(lhs.id, rhs.id);
    }
  };
} // namespace BWAPI

