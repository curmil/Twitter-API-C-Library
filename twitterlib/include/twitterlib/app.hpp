#ifndef TWITTERLIB_APP_HPP
#define TWITTERLIB_APP_HPP
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <networklib/response.hpp>

#include <networklib/stream.hpp>
#include <twitterlib/account.hpp>
#include <twitterlib/objects/tweet.hpp>
#include <twitterlib/objects/user.hpp>
#include <twitterlib/twitter_stream.hpp>

namespace network {
class Request;
}

namespace twitter {

class App {  // Make a struct with public access to members.
   public:
    /// Set the consumer key and consumer secret at construction.
    App(std::string key, std::string secret)
        : consumer_key_{std::move(key)}, consumer_secret_{std::move(secret)} {}

    void set_account(const Account& account);

    /// Returns consumer key.
    std::string key() const { return consumer_key_; }

    /// Returns consumer secret.
    std::string secret() const { return consumer_secret_; }

    /// Set consumer key.
    void set_key(const std::string& key) { consumer_key_ = key; }

    /// Set consumer secret.
    void set_secret(const std::string& secret) { consumer_secret_ = secret; }

    /// Return bearer token.
    std::string bearer_token() const { return bearer_token_; }

    /// Set bearer token value.
    // void set_bearer_token(std::string token) {
    //     bearer_token_ = std::move(token);
    // }

    // boost::asio::io_service& io_service() { return *ios_ptr_; }

    /// Get account associated with this App object.
    Account account() const { return account_; }

    Twitter_stream user_stream{this, build_user_stream_parameters()};
    Twitter_stream filtered_stream{this, build_filtered_stream_parameters()};
    Twitter_stream sample_stream{this, build_sample_stream_parameters()};

    ////////////////////////////////////////////////////////////////////////////
    // - - - REST API - - -
    void update_status(const std::string& message);

    network::Response get_account_settings();

    network::Response verify_credentials(bool include_entities = true,
                                         bool skip_status = false,
                                         bool include_email = false);

    std::vector<std::int64_t> get_blocked_ids();

    std::vector<User> get_blocked_users(bool include_entities = false,
                                        bool skip_status = true);

    // TODO: Change return type to Colletion, that wraps a vector<Tweet> and
    // includes the description data as well.
    network::Response get_collection(const std::string& id,
                                     int count = -1,
                                     int max_position = -1,
                                     int min_position = -1);

    network::Response find_collections(const std::string& screen_name,
                                       std::int64_t user_id = -1,
                                       std::int64_t tweet_id = -1,
                                       int count = -1);

    network::Response get_collection_info(const std::string& id);

    /// Use -1 or empty string for a value you do not wish to specify.
    std::vector<Tweet> get_favorites(const std::string& screen_name,
                                     int count = -1,
                                     bool include_entities = false,
                                     std::int64_t user_id = -1,
                                     std::int64_t since_id = -1,
                                     std::int64_t max_id = -1);

    network::Response get_application_rate_limit_status();

    network::Response get_account_rate_limit_status();

    ////////////////////////////////////////////////////////////////////////////
    // - - - STREAMING API - - -
    // void register_to_user_stream(Stream::Callback callback,
    //                              Stream::Condition condition =
    //                                  [](const network::Response&) { return
    //                                  true; });
    // void register_to_filtered_stream(Stream::Callback callback,
    //                                  Stream::Condition condition =
    //                                      [](const network::Response&) {
    //                                      return true;
    //                                      });

    // void register_to_sample_stream(Stream::Callback callback,
    //                                Stream::Condition condition =
    //                                    [](const network::Response&) { return
    //                                    true; });
    // Public_stream& filtered_stream() { return filtered_stream_; }

   private:
    std::string consumer_key_;
    std::string consumer_secret_;
    std::string bearer_token_;

    // User_stream user_stream_{this};
    // Public_stream filtered_stream_{this, "/1.1/statuses/filter.json",
    // "POST"};
    // Public_stream sample_stream_{this, "/1.1/statuses/sample.json",
    // "GET"};

    Account account_;

    // Sends an Account Authorized request.
    network::Response send(network::Request& request, const Account& account);

    // Sends an application only authentication request.
    network::Response send(network::Request& request);
};

}  // namespace twitter
#endif  // TWITTERLIB_APP_HPP
