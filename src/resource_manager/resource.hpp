#ifndef RESOURCE
#define RESOURCE

#include <string>

/**
 * \brief Abstract resource data class
 * This class hold the data for different types of
 * resource. In order to create a new type of resource
 * just extend from this class
 */
class Resource
{

	friend class ResourceManager;

public:

	enum class Status
	{
		UNKOWN,
		STORED,
		QUEUED,
		LOADING,
		LOADED,
		SAVING
	};

protected:

	std::string name;
	Status status;
public:

	Resource(const std::string& name = "");

	const std::string& getName()const;
    void setName(const std::string& name);

	Resource::Status getStatus()const;

	virtual ~Resource(){};

    operator const char *() const { return name.c_str(); }

};

#endif
