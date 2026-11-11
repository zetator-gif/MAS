#include <any>
#include <future>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class AuthManager {
public:
    virtual ~AuthManager() = default;
};

class SecureFunctionTool {
public:
    virtual ~SecureFunctionTool() = default;
    virtual std::string name() const = 0;
};

struct Message {
    std::string role;
    std::string content;
};

class BaseAgent {
public:
    BaseAgent(std::string agent_id, AuthManager& auth_manager)
        : agent_id_(std::move(agent_id)), auth_manager_(auth_manager) {}

    virtual ~BaseAgent() = default;

    virtual std::string system_prompt() const = 0;

    void register_tool(std::shared_ptr<SecureFunctionTool> tool) {
        if (!tool) {
            throw std::invalid_argument("tool must not be null");
        }
        tools_.push_back(std::move(tool));
    }

    std::vector<std::shared_ptr<SecureFunctionTool>> tools() const {
        return tools_;
    }

    virtual std::future<std::any> run(std::vector<Message> messages) {
        (void)messages;
        throw std::logic_error(
            "run() must be implemented by a subclass or delegated to an agent runtime"
        );
    }

    const std::string& agent_id() const {
        return agent_id_;
    }

protected:
    AuthManager& auth_manager_;

private:
    std::string agent_id_;
    std::vector<std::shared_ptr<SecureFunctionTool>> tools_;
};
