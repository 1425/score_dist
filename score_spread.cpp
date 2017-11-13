#include<fstream>
#include<numeric>
#include<set>
#include "../tba/db.h"
#include "../tba/tba.h"

using namespace std;
using namespace tba;

/*
For each year of competition, calculate what the distribution of scores looks like
2d density chart
*/

#define nyi { cout<<"nyi "<<__LINE__<<"\n"; exit(44); }
#define PRINT(X) { cout<<""#X<<":"<<(X)<<"\n"; }

template<typename A,typename B>
ostream& operator<<(ostream& o,pair<A,B> const& p){
	return o<<"("<<p.first<<","<<p.second<<")";
}

template<typename T>
ostream& operator<<(ostream& o,std::optional<T> const& a){
	if(a) return o<<*a;
	return o<<"NULL";
}

template<typename T>
ostream& operator<<(ostream& o,vector<T> const& a){
	o<<"[ ";
	/*std::ostream_iterator<T> x(o," ");
	std::copy(std::begin(a),std::end(a),x);*/
	for(auto elem:a) o<<elem<<" ";
	return o<<"]";
}

template<typename A,typename B,typename C,typename D>
ostream& operator<<(ostream& o,std::variant<A,B,C,D> const& a){
	return std::visit([&](auto b){ o<<b; },a);
}

/*template<typename Func,typename T>
auto mapf(Func f,vector<T> v){
	vector<decltype(f(v[0]))> r;
	transform(begin(v),end(v),back_inserter(r),f);
	return r;
}*/

template<typename Func,typename T>
auto mapf(Func f,T v){
	vector<decltype(f(*begin(v)))> r;
	transform(begin(v),end(v),back_inserter(r),f);
	return r;
}

#define MAP(A,B) mapf([&](auto a){ return A(a); },B)

template<typename T>
vector<T> flatten(vector<vector<T>> a){
	vector<T> r;
	for(auto &elem:a){
		move(begin(elem),end(elem),back_inserter(r));
	}
	return r;
}

template<typename T>
vector<T>& operator|=(vector<T>& in,T t){
	in.emplace_back(move(t));
	return in;
}

template<typename T>
vector<T> non_null(vector<optional<T>> in){
	vector<T> r;
	for(auto a:in){
		if(a) r|=*a;
	}
	return r;
}

template<typename A,typename B>
vector<A> firsts(vector<pair<A,B>> const& in){
	return MAP([](auto a){ return a.first; },in);
}

template<typename A,typename B>
vector<B> seconds(vector<pair<A,B>> const& in){
	return MAP([](auto a){ return a.second; },in);
}

template<typename T>
vector<T> operator+(vector<T> a,vector<T> b){
	move(begin(b),end(b),back_inserter(a));
	return a;
}

template<typename T>
optional<T> min(vector<T> const& a){
	return std::accumulate(
		begin(a),end(a),optional<T>{},
		[](std::optional<T> a,T b)->optional<T>{
			if(!a) return b;
			return min(*a,b);
		}
	);
}

template<typename T>
optional<T> max(vector<T> const& a){
	return std::accumulate(
		begin(a),end(a),optional<T>{},
		[](std::optional<T> a,T b)->optional<T>{
			if(!a) return b;
			return max(*a,b);
		}
	);
}

template<typename T>
class Nonempty_vector{
	vector<T> data;

	public:
	Nonempty_vector(vector<T> in):data(move(in)){
		if(data.empty()){
			throw invalid_argument("empty");
		}
	}

	auto begin(){ return data.begin(); }
	auto end(){ return data.end(); }
};

template<typename T>
Nonempty_vector<T> make_nonempty(vector<T> v){
	return Nonempty_vector<T>(v);
}

template<typename T>
T min(Nonempty_vector<T> a){
	return std::accumulate(
		begin(a),end(a),
		*begin(a),
		[](T a,T b){
			return min(a,b);
		}
	);
}

template<typename T>
T max(Nonempty_vector<T> a){
	return std::accumulate(
		begin(a),end(a),
		*begin(a),
		[](T a,T b){
			return max(a,b);
		}
	);
}

template<typename T>
vector<T> range(T a,T b){
	vector<T> r;
	for(auto at=a;at!=b;at++){
		r|=at;
	}
	return r;
}

template<typename T>
optional<array<T,5>> quartiles(vector<T> in){
	if(in.empty()) return {};

	sort(begin(in),end(in));
	return array<T,5>{
		in[0],
		in[in.size()/4],
		in[in.size()/2],
		in[in.size()*3/4],
		in[in.size()-1]
	};
}

template<typename T,size_t LEN>
ostream& operator<<(ostream& o,array<T,LEN> const& a){
	o<<"[ ";
	for(auto const& elem:a){
		o<<elem<<" ";
	}
	return o<<"]";
}

template<typename T>
vector<T> take(size_t len,vector<T> in){
	vector<T> r;
	copy_n(begin(in),min(in.size(),len),back_inserter(r));
	return r;
}

template<typename T>
vector<T> sorted(vector<T> a){
	sort(begin(a),end(a));
	return a;
}

template<typename T>
vector<T> reversed(vector<T> a){
	reverse(begin(a),end(a));
	return a;
}

template<typename T>
pair<T,T> sorted(pair<T,T> p){
	if(p.second<p.first) swap(p.first,p.second);
	return p;
}

template<typename T>
multiset<T> to_multiset(vector<T> in){
	return multiset<T>{begin(in),end(in)};
}

template<typename T>
size_t max_count(std::multiset<T> const& a){
	auto m=max(mapf([&](auto x){ return a.count(x); },a));
	if(!m) return 0;
	return *m;
}

template<typename T>
std::string as_string(T t){
	std::ostringstream ss;
	ss<<t;
	return ss.str();
}

vector<string> split(string haystack,char needle){
	vector<string> out;
	ostringstream ss;
	for(auto c:haystack){
		if(c==needle){
			out|=ss.str();
			ss.str("");
		}else{
			ss<<c;
		}
	}
	out|=ss.str();
	return out;
}

auto system(string const& s){
	return system(s.c_str());
}

void convert_to_png(string const& filename){
	auto new_filename=split(filename,'.')[0]+".png";
	//the program "convert" comes from Graphics Magick
	//WARNING: Assumes that the input filename is "safe".  
	int r=system("convert "+filename+" "+new_filename);
	if(r){
		throw std::system_error(r,generic_category(),"Call to \"convert\" failed");
	}
}

//start program-specific stuff

template<typename Fetcher>
vector<Match> matches(Fetcher &fetcher,Year year){
	return flatten(
		mapf(
			[&](Event_key event_key){
				return event_matches(fetcher,event_key);
			},
			events_keys(fetcher,year)
		)
	);
}

using Match_score=pair<int,int>;

int score(Match_Score_Breakdown_2017_Alliance const& a){
	return a.totalPoints;
}

Match_score to_match_score(Match_Score_Breakdown_2017 const& a){
	return make_pair(score(a.red),score(a.blue));
}

int score(Match_Score_Breakdown_2016_Alliance const& a){
	return a.totalPoints;
}

Match_score to_match_score(Match_Score_Breakdown_2016 const& a){
	return make_pair(score(a.red),score(a.blue));
}

auto score(Match_Score_Breakdown_2015_Alliance const& a){
	return a.total_points;
}

optional<Match_score> to_match_score(Match_Score_Breakdown_2015 const& a){
	//ignore coopertition
	//return make_pair(score(a.red),score(a.blue));
	auto r1=score(a.red);
	auto r2=score(a.blue);
	if(r1 && r2) return make_pair(*r1,*r2);
	if(!r1 && !r2) return {};
	nyi
}

int score(Match_Score_Breakdown_2014_Alliance const& a){
	return a.assist+a.auto_pts+a.teleop_goal_and_foul+a.truss_and_catch;
}

Match_score to_match_score(Match_Score_Breakdown_2014 const& a){
	return make_pair(score(a.red),score(a.blue));
}

optional<Match_score> to_match_score(Match_Score_Breakdown const& a){
	return visit([](auto b){ return optional<Match_score>{to_match_score(b)}; },a);
}

std::optional<Match_score> to_match_score(std::optional<Match_Score_Breakdown> const& a){
	if(a) return to_match_score(*a);
	return {};
}

auto to_match_score(Match const& m){
	return to_match_score(m.score_breakdown);
}

template<typename Fetcher>
auto match_scores(Fetcher &fetcher,Year year){
	return MAP(to_match_score,matches(fetcher,year));
}

template<typename Fetcher>
int graph_odds(Fetcher &fetcher,Year year){
	auto m=match_scores(fetcher,year);
	auto n=non_null(m);
	auto xs=firsts(n);
	auto ys=seconds(n);
	auto scores=xs+ys;
	PRINT(min(scores));
	PRINT(max(scores));
	PRINT(quartiles(scores));
	auto s=sorted(MAP(sorted,n));
	PRINT(take(5,s));
	PRINT(take(5,reversed(s)));
	auto c=to_multiset(s);

	if(scores.empty()){
		cout<<"No match data\n";
		return 0;
	}
	auto scores_nonempty=make_nonempty(scores);
	PRINT(scores.size());

	const auto score_range=range(min(scores_nonempty),max(scores_nonempty)+1);
	{
		ofstream out("scores_matrix_"+as_string(year)+".csv");
		for(auto losing_score:score_range){
			for(auto winning_score:score_range){
				out<<c.count(make_pair(losing_score,winning_score))<<",";
			}
			out<<"\n";
		}
	}

	{
		ofstream out("scores_3col_"+as_string(year)+".csv");
		for(auto losing_score:score_range){
			for(auto winning_score:score_range){
				out<<losing_score<<","<<winning_score<<","<<c.count(make_pair(losing_score,winning_score))<<"\n";
			}
		}
	}

	{
		auto filename="score_density_"+as_string(year)+".pgm";
		{
			ofstream out(filename);
			out<<"P2\n";
			out<<score_range.size()<<" "<<score_range.size()<<"\n";
			out<<max_count(c)<<"\n";
			for(auto winning_score:reversed(score_range)){
				for(auto losing_score:score_range){
					auto value=c.count(make_pair(losing_score,winning_score));
					out<<min(value,size_t(255))<<" ";
				}
				out<<"\n";
			}
		}
		convert_to_png(filename);
	}
	
	{
		auto filename="score_appears_"+as_string(year)+".pgm";
		{
			ofstream out(filename);
			out<<"P2\n";
			out<<score_range.size()<<" "<<score_range.size()<<"\n";
			out<<1<<"\n";
			for(auto winning_score:reversed(score_range)){
				for(auto losing_score:score_range){
					auto value=c.count(make_pair(losing_score,winning_score));
					out<<min(value,size_t(1))<<" ";
				}
				out<<"\n";
			}
		}
		convert_to_png(filename);
	}
	return 0;
}

int main(){
	string tba_key;
	{
		std::ifstream f("../tba/auth_key");
		assert(f.good());
		getline(f,tba_key);
	}
	Cached_fetcher f{Fetcher{Nonempty_string{tba_key}},Cache{"../tba/cache.db"}};

	auto years=range(Year{1992},Year{2018});
	for(auto year:years){
		PRINT(year);
		graph_odds(f,year);
	}
}
