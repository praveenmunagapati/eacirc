#ifndef CAESARPROJECT_H
#define CAESARPROJECT_H

#include "projects/IProject.h"
#include "Encryptor.h"
#include "CaesarConstants.h"

class CaesarProject : public IProject {
    //! object providing CAESAR functions
    Encryptor* m_encryptor;
    //! settings for CAESAR project
    CAESAR_SETTINGS m_caesarSettings;

public:
    /** constructor, memory allocation
      */
    CaesarProject();

    /** destructor, memory deallocation
      */
    ~CaesarProject();

    string shortDescription() const;

    /** return configuration used for testing
     * @return testing configuration for project (project root)
     */
    static string testingConfiguration();

    /** load project-specific configuration from config file
      * @param pRoot    allocated project config file XML root (corresponding to EACIRC element)
      * @return status
      */
    int loadProjectConfiguration(TiXmlNode* pRoot);

    /** project initialization
      * - create Encryptor
      * - create project-specifiec header in test vector files (if needed)
      * - allocated project specific evaluator (if used)
      * @return status
      */
    int initializeProject();

    /** init IV and key, if set to be initialized only once
      * @return status
      */
    int initializeProjectState();

    /** save state - key, IV (if loadable)
      * @param pRoot    allocated project state XML root
      * @return status
      */
    int saveProjectState(TiXmlNode* pRoot) const;

    /** load state - key, IV (if loadable)
      * @param pRoot    allocated project state XML root
      * @return status
      */
    int loadProjectState(TiXmlNode* pRoot);

    /** create headers in testVectorFile and humanReadableTestVectorFile
      * - called only if saving test vectors, no need to doublecheck
      * @return status
      */
    int createTestVectorFilesHeaders() const;

    /** prepares complete test vector set set
      * @return status
      */
    int generateTestVectors();
};

#endif // CAESARPROJECT_H
