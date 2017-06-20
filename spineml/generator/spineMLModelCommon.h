#pragma once

// Standard includes
#include <functional>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// pugixml includes
#include "pugixml/pugixml.hpp"

// GeNN includes
#include "codeStream.h"
#include "newModels.h"

//----------------------------------------------------------------------------
// SpineMLGenerator::ParamValues
//----------------------------------------------------------------------------
namespace SpineMLGenerator
{
class ParamValues
{
public:
    ParamValues(const std::map<std::string, double> &values, const NewModels::Base &model)
        : m_Values(values), m_Model(model){}

    //----------------------------------------------------------------------------
    // Public API
    //----------------------------------------------------------------------------
    std::vector<double> getValues() const;

private:
    //----------------------------------------------------------------------------
    // Members
    //----------------------------------------------------------------------------
    const std::map<std::string, double> &m_Values;
    const NewModels::Base &m_Model;
};

//------------------------------------------------------------------------
// VarValues
//------------------------------------------------------------------------
class VarValues
{
public:
    VarValues(const std::map<std::string, double> &values, const NewModels::Base &model)
        : m_Values(values), m_Model(model){}

    //----------------------------------------------------------------------------
    // Public API
    //----------------------------------------------------------------------------
    std::vector<double> getValues() const;

private:
    //----------------------------------------------------------------------------
    // Members
    //----------------------------------------------------------------------------
    const std::map<std::string, double> &m_Values;
    const NewModels::Base &m_Model;
};

//------------------------------------------------------------------------
// CodeStream
//------------------------------------------------------------------------
class CodeStream : public ::CodeStream
{
public:
    CodeStream() : m_FirstNonEmptyRegime(true), m_CodeStream(m_Stream)
    {
        setSink(m_CurrentRegimeStream);
    }

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    void onRegimeEnd(bool multipleRegimes, unsigned int currentRegimeID);

    std::string str() const{ return m_Stream.str(); }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    //!< Flag used to determine whether this is the first regime written
    bool m_FirstNonEmptyRegime;

    //!< Reference to code stream that will be used to build
    //!< entire GeNN code string e.g. a block of sim code
    std::ostringstream m_Stream;

    //! < Second internal code stream used to correctly intent the
    ::CodeStream m_CodeStream;

    //!< Internal codestream used to write regime code to
    std::ostringstream m_CurrentRegimeStream;

};

//------------------------------------------------------------------------
// ObjectHandler
//------------------------------------------------------------------------
class ObjectHandler
{
public:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual void onObject(const pugi::xml_node &node, unsigned int currentRegimeID,
                          unsigned int targetRegimeID) = 0;
};

//------------------------------------------------------------------------
// ObjectHandlerError
//------------------------------------------------------------------------
class ObjectHandlerError : public ObjectHandler
{
public:
    //------------------------------------------------------------------------
    // ObjectHandler virtuals
    //------------------------------------------------------------------------
    virtual void onObject(const pugi::xml_node &node, unsigned int currentRegimeID,
                          unsigned int targetRegimeID);
};

//------------------------------------------------------------------------
// Helper functions
//------------------------------------------------------------------------
//!< Generate model code from 'componentClass' node using specified object handlers
//!< to process various components e.g. to generate GeNN code strings
bool generateModelCode(const pugi::xml_node &componentClass, ObjectHandler &objectHandlerEvent,
                       ObjectHandler &objectHandlerCondition, ObjectHandler &objectHandlerImpulse,
                       ObjectHandler &objectHandlerTimeDerivative,
                       std::function<void(bool, unsigned int)> regimeEndFunc);

//!< Search through code for references to named variable, rename it and wrap in GeNN's $(XXXX) tags
void wrapAndReplaceVariableNames(std::string &code, const std::string &variableName,
                                 const std::string &replaceVariableName);

//!< Search through code for references to named variable and wrap in GeNN's $(XXXX) tags
void wrapVariableNames(std::string &code, const std::string &variableName);

//!< Based on the set of parameter names which are deemed to be variable,
//!< build vectors of variables and parameters to be used by GeNN model
std::tuple<NewModels::Base::StringVec, NewModels::Base::StringPairVec> findModelVariables(
    const pugi::xml_node &componentClass, const std::set<std::string> &variableParams,
    bool multipleRegimes);

//!< Build vector of analogue receive ports names, optionally applying a suffix to each ones name e.g. for v_post
NewModels::Base::StringVec findAnalogueReceivePortNames(const pugi::xml_node &componentClass,
                                                        const std::string &suffix = "");

void substituteModelVariables(const NewModels::Base::StringVec &paramNames,
                              const NewModels::Base::StringPairVec &vars,
                              const std::vector<std::string*> &codeStrings);

void substituteModelVariables(const NewModels::Base::StringVec &paramNames,
                              const NewModels::Base::StringPairVec &vars,
                              const NewModels::Base::StringVec &analogueReceivePortNames,
                              const std::vector<std::string*> &codeStrings);

std::tuple<NewModels::Base::StringVec, NewModels::Base::StringPairVec> processModelVariables(
    const pugi::xml_node &componentClass, const std::set<std::string> &variableParams,
    bool multipleRegimes, const std::vector<std::string*> &codeStrings);

}   // namespace SpineMLGenerator