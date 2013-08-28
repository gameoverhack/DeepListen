//
//  VectorUtils.h
//  DeepListen
//
//  Created by game over on 1/04/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#ifndef _H_VECTORUTILS
#define _H_VECTORUTILS

#include <algorithm>
#include <vector>

inline int secondsToFrames(int seconds){
    return seconds * 25;
}

inline int millisToFrames(int millis){
    return ceil( (float)millis / 1000.0f ) * 25.0f;
}

inline int minutesToFrames(int minutes){
    return minutes * 60.0f * 25.0f;
}

inline int minutesToMillis(float minutes){
    return minutes * 60.0f * 1000.0f;
}

inline int framesToMillis(int frames){
    return ceil( ( (float)frames/25.0f ) * 1000);
}

inline float framesToMinutes(int frames){
    return (float)frames / 25.0f / 60.0f;
}

inline float millisToMinutes(int millis){
    return ((float)millis / 1000.0f / 60.0f);
}

template<typename T>
inline ostream& operator<<(ostream& os, vector<T> &v){
    for(int i = 0; i < v.size(); i++){
        os << v[i] << (i == v.size() - 1 ? "" : ", ");
    }
	return os;
};

template<typename T>
inline void printVector(vector<T> v){
    cout << v;
};

template<typename T>
inline vector<T> arrayToVec(T * a, int size){
    vector<T> v(a, a + size);
    return v;
};

//float d[4] = {0.66666f, 0.1f, 0.033333f, 0.2f};
//int e[4] = {0, 1, 2, 3};
//int choice = getRandomElementFromDistribution(d, e, 4, 100);
//stats.insert(pair<int, int>(choice, choice));
//for(int i = 0; i < 4; i++){
//    cout << i << " " << stats.count(i) << " " << (float)stats.count(i)/stats.size() << endl;
//}

template<typename T>
inline T getRandomElementFromDistribution(vector<float>& distribution, vector<T>& elements, int precision = 100){
    assert(distribution.size() == elements.size());
    float sumdistributions = 0;
    vector<int> elementdistribution;
    //elementdistribution.resize(distribution.size() * precision);
    for(int i = 0; i < distribution.size(); i++){
        sumdistributions += distribution[i];
        for(int j = 0; j < distribution[i] * precision; j++){
            elementdistribution.push_back(i);
        }
    }
    //assert((1.0f * precision) - (precision * sumdistributions) <= 1.0f/precision);
    return elements[elementdistribution[ofRandom(elementdistribution.size())]];
};

template<typename T>
inline T getRandomElementFromDistribution(float * distribution, T * elements, int size, int precision = 100){
    vector<float> d = arrayToVec(distribution, size);
    vector<T> e = arrayToVec(elements, size);
    return getRandomElementFromDistribution(d, e, precision);
};

template<typename T>
inline T getRandomElementFromDistribution(float * distribution, vector<T>& elements, int precision = 100){
    return getRandomElementFromDistribution(arrayToVec(distribution, elements.size()), elements, precision);
};

inline int getRandomDistribution(int sizeofdistribution, ...){
    vector<float> distribution, elements;
    distribution.resize(sizeofdistribution);
    elements.resize(sizeofdistribution);
    va_list d;
    va_start(d, sizeofdistribution);
    for (int i = 0; i < sizeofdistribution; i++){
        float f = va_arg(d, double);
        distribution[i] = f;
        elements[i] = i;
    }
    va_end(d);
    return getRandomElementFromDistribution(distribution, elements);
};

inline float getRandomRanges(vector<float> ranges){
    assert(ranges.size() % 2 == 0);
    float range = 0;
    for(int i = 1; i < ranges.size() + 1; i = i + 2){
        range += ranges[i] - ranges[i-1];
    }
    float choice = ofRandom(range);
    range = 0;
    for(int i = 1; i < ranges.size() + 1; i = i + 2){
        if(choice >= range && choice <= ranges[i] - ranges[i-1] + range){
            choice += ranges[i] - range - (ranges[i] - ranges[i-1]);
            break;
        }
        range += ranges[i] - ranges[i-1];
    }
    return choice;
};

inline float getRandomRanges(int sizeofranges, ...){
    vector<float> ranges;
    //assert(sizeofranges % 2 == 0);
    ranges.resize(sizeofranges);
    va_list r;
    va_start(r, sizeofranges);
    for (int i = 0; i < sizeofranges; i++){
        float f = va_arg(r, double);
        ranges[i] = f;
    }
    va_end(r);
    return getRandomRanges(ranges);
};

template<typename A, typename B>
inline vector<B> getVectorFromMap(multimap<A, B>& mm){
    vector<B> v;
    for(typename multimap<A, B>::iterator it = mm.begin(); it != mm.end(); ++it){
        v.push_back(it->second);
    }
    return v;
};

template<typename A, typename B>
inline vector<B> getVectorFromMap(map<A, B>& m){
    vector<B> v;
    for(typename map<A, B>::iterator it = m.begin(); it != m.end(); ++it){
        v.push_back(it->second);
    }
    return v;
};

template<typename A, typename B>
inline B& getMapValueFromIndex(multimap<A, B>& mm, int index){
    assert(index < mm.size());
    typename multimap<A, B>::iterator it = mm.begin();
    for(int i = 0; i < index; i++) ++it;
    return it->second;
};

template<typename A, typename B>
inline B& getMapValueFromIndex(map<A, B>& m, int index){
    assert(index < m.size());
    typename map<A, B>::iterator it = m.begin();
    for(int i = 0; i < index; i++) ++it;
    return it->second;
};

template<typename T>
inline int getVecIndexFromValue(vector<T> & v, T & value){
    int index = -1;
    for(int i = 0; i < v.size(); i++){
        if(v[i] == value){
            index = i;
            break;
        }
    }
    return index;
}

template<typename T>
inline vector<T> getVecIntersection(vector<T> a, vector<T> b){
    vector<T> diff;
    diff.resize(a.size() + b.size());
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    typename vector<T>::iterator it = set_intersection(a.begin(), a.begin() + a.size(), b.begin(), b.begin() + b.size(), diff.begin());
    diff.resize(it - diff.begin());
    return diff;
};

template<typename T>
inline vector<T> getVecUnion(vector<T> a, vector<T> b){
    vector<T> diff;
    diff.resize(a.size() + b.size());
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    typename vector<T>::iterator it = set_union(a.begin(), a.begin() + a.size(), b.begin(), b.begin() + b.size(), diff.begin());
    diff.resize(it - diff.begin());
    return diff;
};

template<typename T>
inline vector<T> getVecDifference(vector<T> a, vector<T> b){
    vector<T> diff;
    diff.resize(a.size() + b.size());
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    typename vector<T>::iterator it = set_difference(a.begin(), a.begin() + a.size(), b.begin(), b.begin() + b.size(), diff.begin());
    diff.resize(it - diff.begin());
    return diff;
};

template<typename T, typename C>
inline vector<T> getVecDifference(vector<T> a, vector<T> b, C c){
    vector<T> diff;
    diff.resize(a.size() + b.size());
    sort(a.begin(), a.end(), c);
    sort(b.begin(), b.end(), c);
    typename vector<T>::iterator it = set_difference(a.begin(), a.begin() + a.size(), b.begin(), b.begin() + b.size(), diff.begin());
    diff.resize(it - diff.begin());
    return diff;
};

template<typename T>
inline vector<T> uniqueRandomVec(vector<T> a){
    // TODO
}
        
inline void uniqueRandomIndex(vector<int> & vec, int start, int end, int size){
    
    int r = (int)ofRandom(start, end + 1);
    bool bInVec = false;
    
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] == r){
            bInVec = true;
            break;
        }
    }
    
    if(!bInVec) vec.push_back(r);
    
    if(vec.size() != size && end - start + 1 >= size) uniqueRandomIndex(vec, start, end, size);
    
}
        
#endif
