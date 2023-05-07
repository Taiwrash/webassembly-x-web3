#include <eosio/eosio.hpp>

class [[eosio::contract]] my_contract : public eosio::contract {
public:
   using eosio::contract::contract;

   [[eosio::action]]
   void setmessage(std::string message) {
      require_auth(get_self());

      message_table messages(get_self(), get_first_receiver().value);
      auto iterator = messages.find(get_self().value);

      if (iterator == messages.end()) {
        messages.emplace(get_self(), [&](auto& row) {
           row.key = get_self();
           row.message = message;
        });
     } else {
        messages.modify(iterator, get_self(), [&](auto& row) {
           row.message = message;
        });
     }
  }

  [[eosio::action]]
  std::string getmessage() {
     message_table messages(get_self(), get_first_receiver().value);
     auto iterator = messages.find(get_self().value);
     if (iterator == messages.end()) {
        return "No message found.";
     } else {
        return iterator->message;
     }
  }
private:
struct [[eosio::table]] message_row {
uint64_t key;
std::string message;
     uint64_t primary_key() const { return key; }
  };
  typedef eosio::multi_index<eosio::name("messages"), message_row> message_table;
};

EOSIO_DISPATCH(my_contract, (setmessage)(getmessage));

