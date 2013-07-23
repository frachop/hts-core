#include "common.h"
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/xmlschemastypes.h>
#include <libxml/xmlschemas.h>
#include <boost/filesystem.hpp>

namespace HTS_NAMESPACE {
namespace xml {



	/**
	* Valider un fichier XML à partir d'un fichier XML Schema
	**/

	void my_xmlSchemaValidityErrorFunc(void * ctx , const char *msg, ...) {

		std::ostringstream * os= (std::ostringstream *)ctx;

		va_list  pa;
		va_start(pa, msg);
		char buffer[1024]; bzero(buffer, sizeof(buffer));
		vsnprintf (buffer, sizeof(buffer)-1, msg, pa);
		va_end(pa);

		(*os) << buffer;
	}


	CValidation::CValidation()
	:	m_status(false)
	{
	}

	CValidation::~CValidation()
	{
	}


	void CValidation::setPathSchema(const std::string & pathSchema)
	{
		clearStatus();
		m_pathSchema = pathSchema;
		m_schema.clear();
	}

	void CValidation::setSchema(const std::string & schema)
	{
		clearStatus();
		m_schema = schema;
		m_pathSchema.clear();
	}

	void CValidation::setPathDocument(const std::string & pathDoc)
	{
		clearStatus();
		m_pathDoc = pathDoc;
		m_doc.clear();
	}

	void CValidation::setDocument(const std::string & document)
	{
		clearStatus();
		m_doc = document;
		m_pathDoc.clear();
	}
	void CValidation::clearStatus()
	{
		m_status = false;
		m_schemaError  .clear();
		m_documentError.clear();

	}

	class CAutoXmlCleanup
	{
	public:
		CAutoXmlCleanup() {}
		~CAutoXmlCleanup(){
			xmlSchemaCleanupTypes();
			xmlCleanupParser();
			xmlMemoryDump();
		}
	};

	bool CValidation::validate()
	{
		clearStatus();
		if (m_schema.empty() && m_pathSchema.empty())
			return false;

		if (m_doc.empty() && m_pathDoc.empty())
			return false;

		CAutoXmlCleanup axc;

		// Lecture du document xml
		xmlDocPtr pDoc(NULL);
		if (m_doc.empty()) {

			if (boost::filesystem::exists(m_pathDoc))
				pDoc =xmlReadFile(m_pathDoc.c_str(),NULL,0);

			/* Si le fichier XML ne peut pas être ouvert on signale l'erreur*/
			if (pDoc == NULL) {
				m_documentError  = "Could not parse " + m_pathDoc;
				return false;
			}
		} else {
			pDoc = xmlReadMemory(m_doc.c_str(), m_doc.length(), "noname.xml", NULL, 0);
		}

		// Lecture du schema xml
		xmlSchemaParserCtxtPtr spc = NULL;
		if (m_schema.empty())
			spc= xmlSchemaNewParserCtxt(m_pathSchema.c_str());
		else
			spc = xmlSchemaNewMemParserCtxt(m_schema.c_str(), m_schema.length());

		std::ostringstream schemaError;
		std::ostringstream documentError;

		/* Ouverture du fichier XML Schema */
		xmlSchemaSetParserErrors(spc,
			(xmlSchemaValidityErrorFunc) my_xmlSchemaValidityErrorFunc,
			(xmlSchemaValidityWarningFunc) my_xmlSchemaValidityErrorFunc,
			&schemaError);

		xmlSchemaPtr ptr_schema = xmlSchemaParse(spc);
		xmlSchemaFreeParserCtxt(spc); spc= NULL;
		m_schemaError = schemaError.str();

		/* Si le XML Schema n'a pas pu être chargé*/
		if (ptr_schema == NULL) {
			m_schemaError  = "Could not parse XML Schema";
			return false;
		}

		/* Chargement du contenu du XML SChema  */
		xmlSchemaValidCtxtPtr spvc = xmlSchemaNewValidCtxt(ptr_schema);
		xmlSchemaSetValidErrors(spvc,
			(xmlSchemaValidityErrorFunc) my_xmlSchemaValidityErrorFunc,
			(xmlSchemaValidityWarningFunc) my_xmlSchemaValidityErrorFunc,
			&documentError );

		/* On valide le fichier XML à partir de la structure du XML Schema*/
		const int result = xmlSchemaValidateDoc(spvc, pDoc);
		m_documentError = documentError.str();

		m_status= (result == 0);
		return m_status;
	}

}}
