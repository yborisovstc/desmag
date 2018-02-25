#include <cassert>

#include "uri.h"
#include "chromo.h"

using namespace desmag;

// URI
// Uri conforms to RFC 3986 
// Query syntax: (attr_name '=' attr_value) *( ('&' | '|')  (attr_name '=' attr_value))  

const string Uri::KTypeAny = "*";
const string Uri::KTypeAnywhere = "**";
const string KTypeUnknown = "";
const string Uri::KUpperLevel = "..";

const char Uri:: KSchemeSep = ':';
const char Uri::KPathDelim = '/';
const char Uri::KQueryDelim = '?';
const char Uri::KBaseSep = '#';
const char Uri::KIfaceSep = '%';
const string Uri::KIfaceSepS = "%";
const char Uri::KParentSep = ':';
const char Uri::KSepNone = ' ';
const char Uri::KNodeSep = '/';
const char KGroupStart = '(';
const char KGroupEnd = ')';
const char Uri::KCurUnit = '.';
const string KGroupMark = "()";
const string KRelTypesMarks = "/:";

bool Uri::iInit = false;


Uri::TRel Uri::getRel(char aRchar)
{
    assert(aRchar == KParentSep || aRchar == KNodeSep);
    return aRchar == KNodeSep ? EOwned : EChild;
}

char Uri::getRelChar(TRel aRel)
{
    return aRel == EOwned ? KNodeSep : KParentSep;
}

Uri::Uri(const Uri& s): iUri(s.iUri), iScheme(s.iScheme), iAuthority(s.iAuthority), iPath(s.iPath), iBase(s.iBase), iElems(s.iElems),
    iErr(s.iErr)
{
}

Uri::Uri(const string& aSrc): iUri(aSrc), iErr(false)
{
    Parse();
}

Uri::Uri(): iUri(), iErr(false)
{
}

bool Uri::operator==(const Uri& s) const
{
    return iUri == s.iUri && iScheme == s.iScheme && iAuthority == s.iAuthority &&
	iPath == s.iPath && iBase == s.iBase && iElems == s.iElems && iErr == s.iErr;
}

const string& Uri::Scheme() const
{
    return iScheme;
}

const string& Uri::Authority() const
{
    return iAuthority;
}

const string& Uri::Path() const
{
    return iPath;
}

const string& Uri::GetBase() const
{
    return iBase;
}

string Uri::GetName() const
{
    int size = iElems.size();
    return iElems.empty() ? string() : iElems.rbegin()->mName;
}

void Uri::Append(const Uri& aUri)
{
    for (const_elem_iter it = aUri.Elems().begin(); it != aUri.Elems().end(); it++) {
	AppendElem(*it);
    }
}

void Uri::Prepend(const Uri& aUri)
{
    for (vector<TElem>::const_reverse_iterator it = aUri.Elems().rbegin(); it != aUri.Elems().rend(); it++) {
	PrependElem(*it);
    }
}

void Uri::AppendTail(const Uri& aUri, const_elem_iter aIter)
{
    for (const_elem_iter it = aIter; it != aUri.Elems().end(); it++) {
	AppendElem(*it);
    }
}

void Uri::AppendElem(const string& aName, TRel aRel)
{
    iElems.push_back(TElem(aName, aRel));
}

void Uri::AppendElem(const TElem& aElem)
{
    iElems.push_back(aElem);
}

void Uri::PrependElem(const TElem& aElem)
{
    iElems.insert(iElems.begin(), aElem);
}

void Uri::PrependElem(const string& aName, TRel aRel)
{
    iElems.insert(iElems.begin(), TElem(aName, aRel));
}

bool Uri::IsAbsolute() const
{
    bool res = false;
    if (!iElems.empty()) {
	res = iElems.begin()->mName.empty();
    }
    return res;
}

bool Uri::Compare(const_elem_iter aStart, const Uri& aUri, const_elem_iter& aResPos) const
{
    bool res = false;
    bool matched = true;
    const_elem_iter itb = aStart;
    for (const_elem_iter itf = aUri.Elems().begin(); itf != aUri.Elems().end() && matched; itf++, itb++) {
	matched = (*itb == *itf);
    }
    if (matched) {
	aResPos = itb;
	res = true;
    }
    return res;
}

Uri::const_elem_iter Uri::Begin() const
{
    return iElems.begin();
}

void Uri::Parse()
{
    bool fin = false;
    bool err = false;
    string frag;
    size_t base_end = iUri.find_first_of(KBaseSep, 0);
    if (base_end != string::npos) {
	// Base part is presented
	iBase = iUri.substr(0, base_end);
	frag = iUri.substr(base_end + 1);
	size_t scheme_end = iBase.find_first_of(KSchemeSep, 0);
	if (scheme_end != string::npos) {
	    iScheme = iBase.substr(0, scheme_end);
	}
	size_t auth_beg = (scheme_end != string::npos) ? scheme_end + 1 : 0;
	size_t auth_end = auth_beg;
	// Checking authority
	if ((iBase.size() > (scheme_end + 2)) && (iBase.at(scheme_end + 1) == KPathDelim)
		&& (iBase.at(scheme_end + 2) == KPathDelim)) {
	    auth_beg = auth_beg + 2;
	    auth_end = iBase.find_first_of(KPathDelim, auth_beg);
	    iAuthority = iBase.substr(auth_beg, (auth_end == string::npos) ? string::npos : auth_end - auth_beg);
	}
	// Checking path
	size_t path_beg = (auth_end != string::npos) ? auth_end + 1 : 0;
	size_t path_end = iBase.find_first_of(KQueryDelim, path_beg);
	iPath = iBase.substr(path_beg, (path_end == string::npos) ? string::npos : path_end - path_beg);
    }
    else {
	// Base part is missing
	frag = iUri;
    }
    size_t query_beg = frag.find_first_of('?', 0);
    string hier = frag.substr(0, query_beg);
    // Path
    // Check the relation symbol, if omitted that assume it's hier
    if (!hier.empty()) {
	size_t pos = 0;
	char rtype = KSepNone;
	char rs = hier.at(pos);
	bool curunit = false;
	if (rs == KCurUnit) {
	    curunit = true;
	}
	if (hier.size() > 1) {
	    char rs1 = hier.at(pos + 1);
	    if (rs == KCurUnit && rs1 != KCurUnit) {
		curunit = true;
		pos += 1;
	    }
	    rs = hier.at(pos);
	    size_t rs_pos = KRelTypesMarks.find_first_of(rs, 0);
	    if (rs_pos != string::npos) {
		rtype = rs;
		pos += 1;
	    }
	    else if (!curunit) {
		curunit = true;
	    }
	    if (!curunit) {
		// Root
		AppendElem("", rtype);
	    }
	    do {
		// Selecting next unit
		if (!err) {
		    size_t name_end = hier.find_first_of(KRelTypesMarks, pos);
		    string name = hier.substr(pos, name_end - pos);
		    if (name == KUpperLevel && rtype == KSepNone) {
			err = true; break;
		    }
		    AppendElem(name, rtype);
		    pos = name_end;
		    if (pos != string::npos) {
			rtype = hier.at(pos);
			pos += 1;
		    }
		}
	    } while (!err && pos != string::npos && pos < hier.size());
	}
	if (!iErr && err) {
	    iErr = err;
	}
    }
}

string Uri::DoGetUri(const_elem_iter aStart, const_elem_iter aEnd) const
{
    string res;
    // Hier
    for (vector<Uri::TElem>::const_iterator it = aStart; it != aEnd; it++) {
	Uri::TElem elem = *it;
	if (it == aStart && (!elem.mName.empty()) && elem.mRel != EOwned) {
	    // Current node
	    res.append(1, KCurUnit);
	    res.append(1, elem.relChar());
	}
	res.append(elem.mName);
	if (it + 1 != iElems.end()) {
	    res.append(1, elem.relChar());
	}
    }
    return res;
}

Uri Uri::operator+(const Uri& aUri)
{
    Uri res(*this);
    res.Append(aUri);
    return res;
}

Uri& Uri::operator+=(const Uri& aUri)
{
    Append(aUri);
    return *this;
}

