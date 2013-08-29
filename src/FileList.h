//
//  AssetLoader.h
//  DeepListen
//
//  Created by game over on 31/03/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#ifndef _H_ASSETLOADER
#define _H_ASSETLOADER

#include "ofMain.h"
#include "ofxLogger.h"
#include "SerializationUtils.h"
#include "VectorUtils.h"

#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"

using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;

class File {
    
public:
    
    File(){};
    ~File(){};
    
    string name;
    string fullname;
    string extension;
    string enclosingfolder;
    string path;
    string date;
    int size;
    
    friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & BOOST_SERIALIZATION_NVP(name);
        ar & BOOST_SERIALIZATION_NVP(fullname);
        ar & BOOST_SERIALIZATION_NVP(extension);
        ar & BOOST_SERIALIZATION_NVP(enclosingfolder);
        ar & BOOST_SERIALIZATION_NVP(path);
        ar & BOOST_SERIALIZATION_NVP(date);
        ar & BOOST_SERIALIZATION_NVP(size);
	};
    
    bool operator == (const File& rhs) const{
        return (fullname == rhs.fullname &&
                path == rhs.path &&
                date == rhs.date &&
                size == rhs.size);
    };
    
    bool operator != (const File& rhs) const{
        return (fullname != rhs.fullname ||
                path != rhs.path ||
                date != rhs.date ||
                size != rhs.size);
    };
    
    friend ostream& operator<< (ostream &os, File &f);
    
};

inline ostream& operator<<(ostream& os, File &f){
	os << f.name << " " << f.extension << " " << f.enclosingfolder << " " << f.date << " " << f.size;// << " " << f.path;
	return os;
    };
    
    class FileList {
        
    public:
        
        FileList(){
        };
        
        ~FileList(){
            clear();
        };
        
        struct Directory {
            string path;
            bool recursive;
        };
        
        void allowExt(string extension){
            extensions.push_back(extension);
        }
        
        void addDir(string path, bool recursive){
            Directory directory;
            directory.path = path;
            directory.recursive = recursive;
            directories.push_back(directory);
        }
        
        void listDir(string path, bool recursive){
            addDir(path, recursive);
            list();
        }
        
        void list(){
            
            for(int i = 0; i < directories.size(); i++){
                
                Directory &directory = directories[i];
                
                ofDirectory fileList;
                
                fileList.listDir(directory.path);
                
                for(int i = 0; i < fileList.size(); i++){
                    if(fileList[i].isDirectory()){
                        ofxLogVerbose() << "Listing Folder: " << fileList.getName(i) << endl;
                        if(directory.recursive) addDir(fileList.getPath(i), directory.recursive);
                    }else{
                        for(int j = 0; j < extensions.size(); j++){
                            if(fileList.getName(i).rfind(extensions[j]) != std::string::npos){
                                File f;
                                f.fullname = fileList.getName(i);
                                f.name = ofSplitString(f.fullname, ".")[0];
                                f.extension = ofSplitString(f.fullname, ".")[1];
                                f.path = fileList.getPath(i);
                                f.enclosingfolder = ofSplitString(f.path, "/")[ofSplitString(f.path, "/").size()-2];
                                f.date = DateTimeFormatter::format(fileList[i].getPocoFile().getLastModified(), DateTimeFormat::SORTABLE_FORMAT);
                                f.size = fileList[i].getPocoFile().getSize();
                                
                                // only allow unique names in an assetloader!!
                                map<string, File>::iterator it = files.find(f.name);
                                assert(it == files.end());
                                
                                files.insert(pair<string, File>(f.name, f));
                                
                            }
                        }
                    }
                }
            }
            
            
        }
        
        File getFile(string name){
            map<string, File>::iterator it = files.find(name);
            if(it == files.end()){
                ofxLogError() << "Requested name out of bounds" << endl;
                return dummy;
            }else{
                return it->second;
            }
        }
        
        File getFile(int index){
            if(index > size()){
                ofxLogError() << "Requested index out of bounds" << endl;
                return dummy;
            }else{
                return getMapValueFromIndex(files, index);
            }
        }
        
        bool getFileExists(string name){
            map<string, File>::iterator it = files.find(name);
            if(it != files.end()){
                return true;
            }else{
                return false;
            }
        }
        
        map<string, File>& getFiles(){
            return files;
        }
        
        vector<File> getFilesAsVec(){
            return getVectorFromMap(files);
        }
        
        int size(){
            return files.size();
        }
        
        void clear(){
            extensions.clear();
            files.clear();
        }
        
        File operator[](string name){
            return getFile(name);
        }
        
        File operator[](int index){
            return getFile(index);
        }
        
    protected:
        
        File dummy;
        
        vector<Directory> directories;
        
        map<string, File> files;
        vector<string> extensions;
        
    private:
        
    };
    
#endif
