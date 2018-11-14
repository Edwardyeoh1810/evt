/**
 *  @file
 *  @copyright defined in evt/LICENSE.txt
 */
#pragma once
#include <string>
#include <boost/noncopyable.hpp>
#include <evt/chain/block_state.hpp>
#include <evt/chain/transaction.hpp>

struct pg_conn;

namespace evt {
namespace chain { namespace contracts {
struct abi_serializer;
}}  // namespace chain::contracts

#define PG_OK   1
#define PG_FAIL 0

using block_ptr    = chain::block_state_ptr;
using trx_recept_t = chain::transaction_receipt;
using trx_t        = chain::signed_transaction;
using action_t     = chain::action;

struct copy_context;

class pg : boost::noncopyable {
public:
    pg() : conn_(nullptr) {}

public:
    int connect(const std::string& conn);
    int close();

public:
    int create_db(const std::string& db);
    int drop_db(const std::string& db);
    int exists_db(const std::string& db);
    int is_table_empty(const std::string& table);
    int prepare_tables();
    int drop_table(const std::string& table);
    int prepare_stmts();

public:
    copy_context new_copy_context();
    void commit_copy_context(copy_context&);

public:
    static int add_block(copy_context&, const block_ptr);
    static int add_trx(copy_context&,
                       const trx_recept_t&,
                       const trx_t&,
                       const std::string& block_id,
                       int block_num,
                       const std::string& ts,
                       const chain::chain_id_type& chain_id,
                       int seq_num,
                       int elapsed,
                       int charge);

    static int add_action(copy_context&, const action_t&, const std::string& block_id, int block_num, const std::string& trx_id, int seq_num, const chain::contracts::abi_serializer&);

    int get_latest_block_id(std::string& block_id);
    int set_block_irreversible(const std::string& block_id);

private:
    int block_copy_to(const std::string& table, const std::string& data);

private:
    pg_conn* conn_;

};



}  // namespace evt