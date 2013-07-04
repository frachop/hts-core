#include <libxml/tree.h>
#include <libxml/xpath.h>

namespace HTS_NAMESPACE {
namespace xml {

	class CDoc;

	//=============================================================================
	//
	//=============================================================================

	class CNode;
	typedef std::vector< CNode > CNodeArray;

	class CNode {
	public:
		CNode();
		CNode(const xmlNodePtr & n);
		CNode(const CNode & src) { *this = src; }
		CNode(const std::string & tagName);
		CNode(const char * tagName);
		CNode(xmlNsPtr ns, const std::string & tagName);
		CNode(xmlNsPtr ns, const char * tagName);
		CNode & operator=(const xmlNodePtr & src);
		CNode & operator=(const CNode & src);
		virtual ~CNode();
		operator xmlNodePtr() const { return m_node; }
		bool operator==(const CNode & src) const { return m_node == src.m_node; }
		bool operator!=(const CNode & src) const { return m_node != src.m_node; }

	public:
		CNode & createComment(const std::string & content);

	public:
		std::string name() const;
		std::string content(bool trim=true) const;
		void setContent(const std::string & content);
		std::string attribut(const std::string & nom, const std::string & default_name="") const;
		xmlAttrPtr setAttribut(const std::string & nom, const std::string & value) const;
		std::string findAndGetValue(const std::string & p) const;

		xmlElementType	type() const { return ( isNull() ? (xmlElementType)0 : m_node->type ); }
		unsigned short	line() const {	return ( isNull() ? 0 : m_node->line ); } // line number

	public: // DATA SECTIONS
		bool insertDataSection(const std::string & content ) const;
		int getDataSectionContentCount() const;
		std::string getDataSectionContent(int index = 0) const;

	public: // TREE
		CNode  children() const { return ( isNull() ? CNode() : CNode( m_node->children ) ); } // parent->childs link
		CNode  last    () const { return ( isNull() ? CNode() : CNode( m_node->last     ) ); } // last child link
		CNode  next    () const { return ( isNull() ? CNode() : CNode( m_node->next     ) ); } // next sibling link
		CNode  prev    () const { return ( isNull() ? CNode() : CNode( m_node->prev     ) ); } // previous sibling link
		CNode  parent  () const { return ( isNull() ? CNode() : CNode( m_node->parent   ) ); } // child->parent link
		xmlDocPtr docPtr  () const { return ( isNull() ? NULL : m_node->doc ); } // the containing document End of common p

	public:
		bool isNull() const { return (m_node == NULL); }
		CNode findFirstPath(const std::string & path) const;
		size_t getChilds(CNodeArray & destination, const std::string & childName = "") const;
		size_t getChildNodesByTagName( CNodeArray & destination, const std::string & tagName = "" ) const;
		bool isMyParent(const CNode & node) const;
	public:
		CNode operator[](const std::string & tagName) const;

	public:
		int childHomonymeIndex() const;
		std::string getAbsoluteLocationPath() const;

	public:
		CNode	newChild(const char * name, const char * content = NULL, xmlNsPtr ns = NULL) const;
		CNode	newChild(const std::string & name, const std::string & content = std::string(""), xmlNsPtr ns = NULL) const;
		CNode	newChild(const char * name, int content, xmlNsPtr ns = NULL) const;
		CNode	newChild(const std::string & name, int content, xmlNsPtr ns = NULL) const;

		bool addChild(xmlNodePtr child) const;
		bool addChild(const CNode & child) const;

	protected:
		xmlNodePtr m_node;
	};

	//-////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CXPath
	{
	public:
		CXPath();
		virtual ~CXPath();

	public:
		bool create( const CDoc & doc );
		size_t query( CNodeArray & res, const std::string & q, const CNode & from = CNode() ) const;

	private:
		void reset();
		xmlXPathContextPtr m_xpathCtx;
	};

	//=============================================================================
	// CValidation
	//=============================================================================

	class CValidation
	{
	public:
		CValidation();
		virtual ~CValidation();

	public:
		void setPathSchema(const std::string & pathSchema);
		void setSchema(const std::string & schema);

	public:
		void setPathDocument(const std::string & pathDoc);
		void setDocument(const std::string & document);

	public:
		bool validate();
		bool getStatus() const { return m_status; }
		std::string getSchemaError() const { return m_schemaError; }
		std::string getDocumentError() const { return m_documentError; }

	private:
		std::string m_pathDoc;
		std::string m_doc;
		std::string m_pathSchema;
		std::string m_schema;

	private:
		bool _validate( void* _ptr_ctxt );
		void clearStatus();
		bool m_status;
		std::string m_schemaError  ;
		std::string m_documentError;
	};

	//=============================================================================
	// CDoc
	//=============================================================================

	class CDoc {

	public:
		CDoc();
		virtual ~CDoc();
		operator xmlDocPtr() const { return (xmlDocPtr)m_doc; }
		xmlDocPtr create(const std::string & version="1.0", const std::string & encoding="" );

	public:
		CNode getRoot() const;
		CNode setRoot(const CNode & root);// Returns:	the old root element if any was found, NULL if root was NULL

		virtual bool load(const std::string & nomFichier);
		virtual bool loadFromMemory(const std::string & document);

		virtual std::string toString() const;
		virtual bool save(const std::string & nomFichier) const;
		virtual void close() { cleanup(); }
		virtual bool isNull() const { return m_doc == NULL; }
		std::string filename() const;

	private:
		void cleanup();
		xmlDocPtr m_doc;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	/// CParserBase
	
	typedef HTS_NAMESPACE::logger::CBase CLoggerBase;

	class CParserBase {
	public:
		CParserBase();
		CParserBase(const CNode & root, CLoggerBase * pLogger=NULL);
		virtual ~CParserBase();
	
	public:
		void setRoot  ( const CNode & root ) { m_root = root; }
		void setLogger( CLoggerBase * pLogger ) { m_pLogger = pLogger; }

	public:
		bool parseChilds();
	
	protected:
		template<typename T> static T fromString(const std::string & , const T & def) { return def; }
	
	protected:
		virtual bool preValidate() { return true; }
		virtual bool parseChild(const CNode & child);
		virtual bool postValidate() { return true; }
	
	protected:
		void log(int facility, const char * format, ...);
		void log(const char * format, ...);

	protected:
		CNode      m_root;
		CLoggerBase * m_pLogger;
	};
	
	/// CParserBase
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	/// CParserBase templates
	
	template<> inline bool CParserBase::fromString(const std::string & s, const bool & def) {
		const std::string _s = HTS_NAMESPACE::string::trim(s);
		const char * ps= _s.c_str();

		if (_s == "1") return  true;
		if (strcasecmp(ps, "yes" ) == 0) return true;
		if (strcasecmp(ps, "y" ) == 0) return true;
		if (strcasecmp(ps, "oui" ) == 0) return true;
		if (strcasecmp(ps, "o" ) == 0) return true;
		if (strcasecmp(ps, "true" ) == 0) return true;

		if (_s == "0") return false;
		if (strcasecmp(ps, "non" ) == 0) return false;
		if (strcasecmp(ps, "no" ) == 0) return false;
		if (strcasecmp(ps, "n" ) == 0) return false;
		if (strcasecmp(ps, "false" ) == 0) return false;

		return def;
	}
	
	/// CParserBase templates
	////////////////////////////////////////////////////////////////////////////////////////////////

}}


