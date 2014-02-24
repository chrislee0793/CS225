/**
 * @file word_counter.cpp
 * Implementation of the WordFreq class.
 *
 * @author Chase Geigle
 * @date Spring 2011
 * @date Spring 2012
 */
 
using std::vector;
using std::pair;
using std::string;
using std::ifstream;
using std::istringstream;
using std::cout;
using std::endl;
 
template <template <class K, class V> class HT>
WordFreq<HT>::WordFreq( const string & infile ) : filename( infile ) { /* nothing */ }
 
template <template <class K, class V> class HT>
vector< pair<string, int> > WordFreq<HT>::getWords( int threshold ) const {
    TextFile infile( filename );
    vector< pair<string, int> > ret;
        HT<string, int> table(256);
        while(infile.good()) {
                table[infile.getNextWord()]++;
        }
 
        typename HT<string, int>::iterator it;
    for( it = table.begin(); it != table.end(); it++ ) {
        if( it->second >= threshold )
            ret.push_back( *it );
    }
    return ret;
}