#ifndef __DESMAG_URI_H
#define __DESMAG_URI_H

#include <utility>
#include <vector>
#include <map>
#include <string>

namespace desmag {

    using namespace std;

    class Uri
    {

	public:
	    enum TRel {
		EOwned = 0,
		EChild
	    };

	    class Unit
	    {
		public:
		    Unit(): mName(), mRel(TRel::EOwned) {}
		    Unit(const string& aName, TRel aRel = TRel::EOwned): mName(aName), mRel(aRel) {}
		    Unit(const Unit& src): mName(src.mName), mRel(src.mRel) {}
		    bool operator ==(const Unit& aPair) const { return mName == aPair.mName && mRel == aPair.mRel;}
		    char relChar() const { return EOwned ? KNodeSep : KParentSep;}
		    string mName;
		    TRel mRel;
	    };
	public:
	    // Relation: relation type, name
	    typedef Unit TElem;
	    typedef vector<TElem>::const_iterator const_elem_iter;
	    typedef vector<TElem>::iterator elem_iter;
	public:
	    Uri();
	    Uri(const Uri& aSrc);
	    Uri(const string& aSrc);
	    const vector<TElem>& Elems() const {return iElems;};
	    string GetUri(const_elem_iter aStart) const { return DoGetUri(aStart, iElems.end());}
	    string GetUri() const { return GetUri(iElems.begin());}
	    const string& Scheme() const;
	    const string& Authority() const;
	    const string& Path() const;
	    string GetName() const;
	    const string& GetBase() const;
	    const_elem_iter Begin() const;
	    void Append(const Uri& aUri);
	    void Prepend(const Uri& aUri);
	    void AppendTail(const Uri& aUri, const_elem_iter aIter);
	    void AppendElem(const string& aName, TRel aRel = TRel::EOwned);
	    void AppendElem(const string& aName, char aRel = KNodeSep) { AppendElem(aName, getRel(aRel));}
	    void AppendElem(const TElem& aElem);
	    void PrependElem(const TElem& aElem);
	    void PrependElem(const string& aName, TRel aRel = TRel::EOwned);
	    bool IsErr() const { return iErr;};
	    bool IsAbsolute() const;
	    bool Compare(const_elem_iter aStart, const Uri& aUri, const_elem_iter& aResPos) const;
	    Uri operator+(const Uri& aUri);
	    Uri& operator+=(const Uri& aUri);
	    bool operator==(const Uri& aSrc) const;
	    operator string() const { return GetUri(iElems.begin());}
	protected:
	    string DoGetUri(const_elem_iter aStart, const_elem_iter aEnd) const;
	    void Parse();
	    static TRel getRel(char aRchar);
	    static char getRelChar(TRel aRel);
	protected:
	    string iUri;
	    string iScheme;
	    string iAuthority;
	    string iPath;
	    // Nw hier part
	    string iBase;
	    vector<TElem> iElems;
	    static bool iInit;
	    bool iErr;
	public:
	    static const string KTypeAny;
	    static const string KTypeAnywhere;
	    static const string KUpperLevel;
	    static const char KParentSep;
	    static const char KSchemeSep;
	    static const char KPathDelim;
	    static const char KQueryDelim;
	    static const char KNodeSep;
	    static const char KSepNone;
	    static const char KBaseSep;
	    static const char KIfaceSep;
	    static const string KIfaceSepS;
	    static const char KCurUnit;
    };

}

#endif
