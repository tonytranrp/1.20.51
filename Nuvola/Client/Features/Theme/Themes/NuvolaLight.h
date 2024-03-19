#include "../Theme.h"

class NuvolaLight : public Theme
{
public:
	NuvolaLight();

	virtual std::string getName() const override;
	virtual void setName(std::string name) override;
	virtual std::string getAuthor() const override;
	virtual void setAuthor(std::string author) override;
};