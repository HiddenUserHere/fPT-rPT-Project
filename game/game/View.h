
class View
{
public:
	View();
	virtual ~View();

	void										OnDraw();
};

typedef std::shared_ptr<View> View_ptr;