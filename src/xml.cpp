#include "common.h"
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/xmlschemastypes.h>
#include <libxml/xmlschemas.h>

typedef const char * PCSTR;
#define XMLCHAR (xmlChar *)

using namespace HTS_NAMESPACE::string;

namespace HTS_NAMESPACE {
namespace xml {

	//=============================================================================
	// CNode
	//=============================================================================

	CNode::CNode()
	:	m_node(NULL)
	{
	}

	CNode::CNode(const xmlNodePtr & n)
	:	m_node(n)
	{
	}

	CNode & CNode::operator=(const CNode & src) {
		if (this != &src) {
			m_node = src.m_node;
		}
		return *this;
	}

	CNode::CNode(const std::string & tagName)
	:	m_node( xmlNewNode(NULL, XMLCHAR tagName.c_str()) )
	{
	}

	CNode::CNode(const char * tagName)
	:	m_node( xmlNewNode(NULL, XMLCHAR tagName) )
	{
	}

	CNode::CNode(xmlNsPtr ns, const std::string & tagName)
	:	m_node( xmlNewNode(ns, XMLCHAR tagName.c_str()) )
	{
	}

	CNode::CNode(xmlNsPtr ns, const char * tagName)
	:	m_node( xmlNewNode(ns, XMLCHAR tagName) )
	{
	}

	CNode & CNode::operator=(const xmlNodePtr & src) {
		m_node = src;
		return (*this);
	}

	CNode::~CNode() {
	}

	CNode& CNode::createComment(const std::string & content) {
		m_node = xmlNewComment(XMLCHAR content.c_str());
		return *this;
	}






	std::string CNode::name() const {
		if (isNull()) return "";
		if (m_node->name)
			return trim( std::string((PCSTR)m_node->name) );
			
		return "";
	}

	std::string CNode::content( bool bTrim ) const {
		if (isNull()) return "";
		PCSTR res= (PCSTR)xmlNodeListGetString((xmlDocPtr)m_node, m_node->xmlChildrenNode, 1);
		std::string _content = std::string( (res==NULL) ? "" : res );
		return (bTrim) ? trim(_content) : _content;
	}

	void CNode::setContent(const std::string & content){
		if (! isNull() )
			xmlNodeSetContent( m_node, (const xmlChar *) content.c_str() );
	}

	std::string CNode::attribut(const std::string & nom, const std::string & default_name) const {
		const xmlChar * resultat = xmlGetProp (m_node, (const xmlChar *) nom.c_str());
		//return strTrim( std::string( (resultat == NULL) ? default_name : (PCSTR)resultat ) );
		return std::string( (resultat == NULL) ? default_name : (PCSTR)resultat );
	}

	xmlAttrPtr CNode::setAttribut(const std::string & nom, const std::string & value) const {
		xmlAttrPtr resultat = xmlSetProp (m_node, (const xmlChar *) nom.c_str(), (const xmlChar *) value.c_str());
		return resultat;
	}

	bool CNode::isMyParent(const CNode & p) const
	{
		if (p.isNull())
			return false;

		if (isNull())
			return false;

		if (m_node == p.m_node)
			return false;

		if (m_node->parent == p.m_node)
			return true;

		return parent().isMyParent( p );

	}

	CNode CNode::findFirstPath(const std::string & p) const {
		if (isNull())
			return CNode();

		std::string newPath(""), path(p);
		size_t pos = path.find('/');

		// enlever l'eventuel / du debut
		if (pos == 0) {
			path = path.substr(1, path.length()-1);
			pos = path.find('/');
		}

		if (pos != std::string::npos ) {
			newPath = path.substr(pos+1, path.length() - pos - 1);
			path = path.substr(0, pos);
		}

		//printf("\nSearching node : '%s'...\n", p.c_str());

		bool bTrouve= false;
		xmlNodePtr node = this->m_node->children; // node <xml>
		while ((!bTrouve) && (node != NULL)) {
			if (node->type == XML_ELEMENT_NODE)
			{
				bTrouve = ( xmlStrcmp((const xmlChar *) node->name, (const xmlChar *)path.c_str()) == 0 ) ;

				//cout << bTrouve << " : xmlStrcmp : " << xmlStrcmp((const xmlChar *) node->name, (const xmlChar *) (path.c_str())) << endl;
				//bTrouve = xmlStrcmp((const xmlChar *) node->name, (const xmlChar *) (path.c_str())) == 0;
			}

			if (!bTrouve)
				node = node->next; // hop! node suivant
		}

		if (!bTrouve)
			return CNode();

		if (newPath.length() == 0)
			return CNode(node);

		return CNode(node).findFirstPath(newPath);
	}

	CNode CNode::operator[](const std::string & tagName) const
	{
		if (tagName.empty() || (isNull()))
			return CNode();

		xmlNodePtr node = this->m_node->children; // node <xml>
		while (node != NULL) {
			if (node->type == XML_ELEMENT_NODE)
				if ( xmlStrcmp((const xmlChar *) node->name, (const xmlChar *)tagName.c_str()) == 0 )
					return CNode(node);

			node = node->next; // hop! node suivant
		}

		return CNode();
	}

	size_t CNode::getChilds(CNodeArray & destination, const std::string & childName) const {
		if (isNull())
			return destination.size();

		xmlNodePtr node = m_node->children;
		while (node != NULL) {
			if (node->type == XML_ELEMENT_NODE) {
				const std::string nodeName = (PCSTR) node->name;

				bool bTrouve = true;
				if (childName.length())
					bTrouve= (childName == nodeName);

				if (bTrouve)
					destination.push_back(CNode(node));
			}

			node = node->next; // hop! node suivant
		}

		return destination.size();
	}

	size_t CNode::getChildNodesByTagName(CNodeArray & destination, const std::string & tagName) const
	{
		getChilds( destination, tagName );

		if (!isNull()) {
			xmlNodePtr node = m_node->children;
			while (node != NULL)
			{
				CNode n = node;
				n.getChildNodesByTagName( destination, tagName );

				node = node->next; // hop! node suivant
			}
		}
		return destination.size();
	}

	std::string CNode::findAndGetValue(const std::string & p) const {
		return findFirstPath(p).content();
	}

	int CNode::childHomonymeIndex() const {
		if (isNull()) return -1;
		const std::string myName= name();
		if (myName.empty()) return -1;

		CNode p = m_node->parent;
		if (p.isNull()) return -1;

		int idx = 0;
		xmlNodePtr node = p.children();
		while (node != NULL) {

			if (node == m_node)
				return idx;

			if (node->type == XML_ELEMENT_NODE)
				if (node->name)
					if (myName == std::string( (const char*) node->name))
						idx++;

			node = node->next; // hop! node suivant
		}

		return idx;

	}
	
	
	std::string CNode::getAbsoluteLocationPath() const {
		if (isNull()) return std::string("");

		std::string result = "";

		CNode p = *this;

		while (!p.isNull()) {

			std::string name = p.name();
			if (name.empty()) {
				result = "/" + result;
				break;
			}

			const int idx = p.childHomonymeIndex();
			if (idx > 0) {
				std::ostringstream oss(std::ostringstream::out);
				oss << '[' << idx << ']';
				name += oss.str();
			}

			if (result.empty())
				result = name;
			else
				result = name + "/" + result;

			p = p.parent();

		};
		
		return result;
		
	}
	
	bool CNode::insertDataSection(const std::string & content ) const
	{
		if (isNull())
			return false;

		xmlNodePtr p = ::xmlNewCDataBlock( docPtr(), (const xmlChar *) content.c_str(), content.size() );
		if (p == NULL)
			return false;

		return (xmlAddChild(m_node, p) == p);
	}


	int CNode::getDataSectionContentCount() const
	{
		if (isNull())
			return 0;

		int res = 0;
		xmlNodePtr node = m_node->children;
		while (node != NULL) {
			if (node->type == XML_CDATA_SECTION_NODE)
				res++;

			node = node->next; // hop! node suivant
		}
		return res;
	}

	std::string CNode::getDataSectionContent(int index) const {
		if (isNull())
			return "";

		xmlNodePtr node = m_node->children;
		while ((node != NULL) && (index >= 0)){
			if (node->type == XML_CDATA_SECTION_NODE) {
				if (index == 0)
					return std::string( (const char*) node->content );
	
				index--;
			}
	
			node = node->next; // hop! node suivant
		}
		return "";
	}

	CNode	CNode::newChild(const char * name, const char * content, xmlNsPtr ns) const {
		if (isNull()) return CNode();
		return CNode( ::xmlNewChild(m_node,ns, XMLCHAR name, XMLCHAR content) );
	}

	CNode	CNode::newChild(const std::string & name, const std::string & content, xmlNsPtr ns) const {
		return newChild(name.c_str(), content.c_str(), ns);
	}

	CNode	CNode::newChild(const char * name, int content, xmlNsPtr ns) const {
		std::ostringstream oss;
		oss << content;
		return newChild(name, oss.str().c_str(), ns);
	}

	CNode	CNode::newChild(const std::string & name, int content, xmlNsPtr ns) const {
		return newChild(name.c_str(), content, ns);
	}

	bool CNode::addChild(xmlNodePtr child) const {
		if (child == NULL) return false;
		if (isNull()) return false;
		return ::xmlAddChild(m_node,child) != NULL;
	}

	bool CNode::addChild(const CNode & child) const {
		return addChild(child.m_node);
	}

	//=============================================================================
	// CDoc
	//=============================================================================

	CDoc::CDoc()
	:	m_doc( NULL )
	{
		xmlInitParser();
	}

	CDoc::~CDoc() {
		cleanup();
		xmlCleanupParser();
		xmlMemoryDump();
	}


	CNode CDoc::getRoot() {
		return CNode( xmlDocGetRootElement(m_doc) );
	}

	CNode CDoc::setRoot(const CNode& n) {
		return CNode( ::xmlDocSetRootElement(m_doc, (xmlNodePtr)n) );
	}



	std::string CDoc::filename() const {
		if (isNull()) return "";
		return std::string( (PCSTR)m_doc->URL );
	}


	void CDoc::cleanup() {
		if (m_doc) {
			xmlFreeDoc(m_doc); // liberation du zozio
			m_doc = NULL;
		}
	}

	xmlDocPtr CDoc::create(const std::string & version, const std::string & encoding ) {
		cleanup();
		m_doc = xmlNewDoc((const xmlChar*)version.c_str());
		if( !encoding.empty() )
			m_doc->encoding = (const xmlChar*)strdup(encoding.c_str());
		return m_doc;
	}

	bool CDoc::load(const std::string & nomFichier) {

		cleanup();

		//FILE * errorOutput= fopen("/dev/null", "w");
		//xmlSetGenericErrorFunc(errorOutput, NULL);

		m_doc = xmlParseFile(nomFichier.c_str());
		
		//fclose( errorOutput );
		
		if (m_doc == NULL) cleanup();
		return (m_doc != NULL);
	}

	bool CDoc::loadFromMemory(const std::string & document)
	{
		cleanup();

		m_doc = xmlReadMemory(document.c_str(), document.length(), "noname.xml", NULL, 0);
	
		if (m_doc == NULL) cleanup();
		return (m_doc != NULL);
	}
	
	std::string CDoc::toString() const {
		std::string res("");
		if (!isNull()) {

			xmlChar *xmlbuff = NULL;
			int buffersize = 0;

			xmlDocDumpFormatMemory(m_doc, &xmlbuff, &buffersize, 1);
			if (xmlbuff) {
				res = (char*) xmlbuff;
				xmlFree(xmlbuff);
			}
		}

		return res;
	}

	bool CDoc::save(const std::string & nomFichier) const {
		std::string xml = toString();
		std::ofstream fOut(nomFichier.c_str());
		if (fOut.good()) {
			fOut << xml;
			fOut.close();
			return fOut.good();
		}
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	/// CParserBase

	CParserBase::CParserBase()
	:	m_pLogger(NULL)
	{
	}
	
	CParserBase::CParserBase(const CNode & root, CLoggerBase * pLogger)
	:	m_root(root)
	,	m_pLogger(pLogger)
	{
	}
	
	CParserBase::~CParserBase() {
	}
	
	bool CParserBase::parseChilds() {

		if (!preValidate()) {
			log( LOG_ERR, "Object is not ready '%s'", m_root.getAbsoluteLocationPath().c_str() );
			return false;
		}

		CNodeArray childs;
		const size_t count = m_root.getChilds(childs);
		for (size_t i= 0; i<count; ++i) {
			CNode child = childs[i];
			if (!parseChild(child)) {
				log( LOG_ERR, "Invalid value '%s'", child.getAbsoluteLocationPath().c_str() );
				return false;
			}
		}
		
		if (!postValidate()) {
			log( LOG_ERR, "Invalid object '%s'", m_root.getAbsoluteLocationPath().c_str() );
			return false;
		}
		return true;
	}


	bool CParserBase::parseChild(const CNode & child) {
		log( LOG_WARNING, "Unused xml tag '%s'", child.getAbsoluteLocationPath().c_str() );
		return true;
	}
	
	void CParserBase::log(int facility, const char * format, ...) {
		if (m_pLogger) {
			va_list argList;
			va_start( argList, format );
			m_pLogger->vlog(format, argList, facility);
			va_end( argList );
		}
	}
	
	void CParserBase::log(const char * format, ...) {
		if (m_pLogger) {
			va_list argList;
			va_start( argList, format );
			m_pLogger->vlog(format, argList);
			va_end( argList );
		}
	}
	
	/// CParserBase
	////////////////////////////////////////////////////////////////////////////////////////////////


	//- XPATH ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CXPath::CXPath()
	:	m_xpathCtx(NULL)
	{

	}
	CXPath::~CXPath()
	{
		reset();
	}

	void CXPath::reset()
	{
		if (m_xpathCtx) xmlXPathFreeContext(m_xpathCtx);
		m_xpathCtx = NULL;
	}

	bool CXPath::create( const CDoc & doc )
	{
		reset();

		/* Create xpath evaluation context */
		m_xpathCtx = xmlXPathNewContext(doc);
		if(m_xpathCtx == NULL) {
			reset();
			return false;
		}
		return true;
	}

	size_t CXPath::query( CNodeArray & res, const std::string & q, const CNode & from ) const
	{
		res.clear();
		if ( m_xpathCtx == NULL)
			return res.size();

		m_xpathCtx->node = from;

		/* Evaluate xpath expression */
		xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)q.c_str(), m_xpathCtx);
		if(xpathObj == NULL)
			return res.size();

		if (!xmlXPathNodeSetIsEmpty(xpathObj->nodesetval))
		{
			ASSERT(xpathObj->nodesetval);
			xmlNodeSetPtr nodes = xpathObj->nodesetval;

			for(int i = 0; i < nodes->nodeNr; ++i) {
				ASSERT(nodes->nodeTab[i]);
				if(nodes->nodeTab[i]->type == XML_ELEMENT_NODE) {
					CNode n(nodes->nodeTab[i]);
					if ( from.isNull() )
						res.push_back( n );
					else if (n.isMyParent(from)) {
						res.push_back( n );
					}
				}
			}
		}
		xmlXPathFreeObject (xpathObj);
		return res.size();
	}



































	#ifdef FOR_TESTS
	static std::string xmlEltType(xmlElementType t) {
		switch(t)  {
			case XML_ELEMENT_NODE      : return "XML_ELEMENT_NODE";
			case XML_ATTRIBUTE_NODE    : return "XML_ATTRIBUTE_NODE";
			case XML_TEXT_NODE         : return "XML_ATTRIBUTE_NODE";
			case XML_CDATA_SECTION_NODE: return "XML_CDATA_SECTION_NODE";
			case XML_ENTITY_REF_NODE	: return "XML_ENTITY_REF_NODE";
			case XML_ENTITY_NODE       : return "XML_ENTITY_NODE";
			case XML_PI_NODE           : return "XML_PI_NODE";
			case XML_COMMENT_NODE      : return "XML_COMMENT_NODE";
			case XML_DOCUMENT_NODE		: return "XML_DOCUMENT_NODE";
			case XML_DOCUMENT_TYPE_NODE: return "XML_DOCUMENT_TYPE_NODE";
			case XML_DOCUMENT_FRAG_NODE: return "XML_DOCUMENT_FRAG_NODE";
			case XML_NOTATION_NODE     : return "XML_NOTATION_NODE";
			case XML_HTML_DOCUMENT_NODE: return "XML_HTML_DOCUMENT_NODE";
			case XML_DTD_NODE          : return "XML_DTD_NODE";
			case XML_ELEMENT_DECL      : return "XML_ELEMENT_DECL";
			case XML_ATTRIBUTE_DECL    : return "XML_ATTRIBUTE_DECL";
			case XML_ENTITY_DECL       : return "XML_ENTITY_DECL";
			case XML_NAMESPACE_DECL    : return "XML_NAMESPACE_DECL";
			case XML_XINCLUDE_START    : return "XML_XINCLUDE_START";
			case XML_XINCLUDE_END      : return "XML_XINCLUDE_END";
			case XML_DOCB_DOCUMENT_NODE: return "XML_DOCB_DOCUMENT_NODE";
		};
		return "???xmlElementType???";
	}

	static void dump(int rg, xmlNodePtr n) {

		std::string tabs;
		for (int i=0; i<3*rg; ++i) tabs += ' ';

		std::string sname= (n->name == NULL) ? "" : ((PCSTR) n->name);
		if ((sname != "text") && (n->type != XML_ATTRIBUTE_NODE)) {

			std::string contenu = "";
			if (n->children == NULL)
				contenu= (PCSTR)xmlNodeListGetString((xmlDocPtr)n, n->xmlChildrenNode, 1);

			contenu = "";
			std::string res= stringFormat(std::string("")
				+tabs + "[%08X] %s : %s = %s\n"
				+tabs + "last   : %08X\n"
				+tabs + "parent : %08X\n"
				+tabs + "next   : %08X\n"
				+tabs + "prev   : %08X\n"
				+tabs + "doc    : %08X\n\n",
				(void*)n,
				xmlEltType(n->type).c_str(),
				(PCSTR) n->name,
				contenu.c_str(),
				(void*)n->children,
				(void*)n->last,		// last child link
				(void*)n->parent,	// child->parent link
				(void*)n->next,		// next sibling link
				(void*)n->prev,		// previous sibling link
				(void*)n->doc		// the containing document

				);
			std::cout << res;
		}

		if (n->children)
			dump(rg+1, n->children);

		if (n->next)
			dump(rg, n->next);
	}
	#endif

}}
