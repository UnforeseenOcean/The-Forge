/*
 * Copyright (c) 2018 Confetti Interactive Inc.
 *
 * This file is part of The-Forge
 * (see https://github.com/ConfettiFX/The-Forge).
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
*/
#pragma once

//Use Virtual FileSystem
//#define USE_VFS

#include "../Interfaces/IOperatingSystem.h"
#include "../../ThirdParty/OpenSource/TinySTL/string.h"
#include "../../ThirdParty/OpenSource/TinySTL/vector.h"

typedef void* FileHandle;
/************************************************************************************************
* If you change this class please check the following projects to ensure you don't break them:  *
* 1) BlackFootBlade																			 *
* 2) Aura																					   *
* 3) Gladiator																				  *
* Also ask yourself if all of those projects will need the change, or just the one you're work- *
* ing on. If it is just the one you're working on please add it to an inherited version of the  *
* class within your project.																	*
************************************************************************************************/

/// Low level file system interface providing basic file I/O operations
/// Implementations platform dependent
FileHandle _openFile(const char* filename, const char* flags);
void _closeFile(FileHandle handle);
void _flushFile(FileHandle handle);
size_t _readFile(void *buffer, size_t byteCount, FileHandle handle);
bool _seekFile(FileHandle handle, long offset, int origin);
long _tellFile(FileHandle handle);
size_t _writeFile(const void *buffer, size_t byteCount, FileHandle handle);
size_t _getFileLastModifiedTime(const char* _fileName);

tinystl::string _getCurrentDir();
tinystl::string _getExePath();
tinystl::string _getAppPrefsDir(const char* org, const char* app);
tinystl::string _getUserDocumentsDir();

void _setCurrentDir(const char* path);

enum FileMode
{
	FM_ReadBinary = 0,
	FM_WriteBinary,
	FM_ReadWriteBinary,
	FM_Read,
	FM_Write,
	FM_ReadWrite,
	FM_Count
};

// TODO : File System should be reworked so that we don't need all of these vague enums.
//	One suggesting is to just have a list of directories to check for each resource type.
//	Applications would then just need to add the paths they need to that list.
enum FSRoot {
	// universal binary shader place
	FSR_BinShaders = 0,
	// the main applications shader source directory
	FSR_SrcShaders,

	// shared shaders binary directory
	FSR_BinShaders_Common,
	// shared shaders source directory
	FSR_SrcShaders_Common,

	// the main application texture source directory (TODO processed texture folder)
	FSR_Textures,

	FSR_Meshes, // Meshes
	FSR_Builtin_Fonts,
	FSR_GpuConfig,
	FSR_OtherFiles,

	// libraries will want there own directories
	// support for upto 100 libraries
	____fsr_lib_counter_begin = FSR_OtherFiles,

	// Add libraries here
	FSR_Lib0_SrcShaders,
	FSR_Lib0_Texture,
	FSR_Lib0_OtherFiles,

	FSR_Lib1_OtherFiles,

	FSR_Lib2_OtherFiles,

	FSR_Lib3_OtherFiles,

	FSR_Lib4_OtherFiles,

	FSR_Lib5_OtherFiles,

	FSR_Lib6_OtherFiles,

	FSR_Lib7_OtherFiles,

	FSR_Lib8_OtherFiles,

	FSR_Lib9_OtherFiles,

	FSR_Lib10_OtherFiles,

	FSR_Lib11_OtherFiles,

	FSR_Lib12_OtherFiles,

	// FSR_Lib1_SrcShaders,
	// FSR_Lib1_Texture,
	// FSR_Lib1_OtherFiles,
	// and etc...

	FSR_Lib_99_SrcShaders = ____fsr_lib_counter_begin + 99 * 3,

	FSR_Absolute,

	FSR_Count
};

enum SeekDir
{
	SEEK_DIR_BEGIN = 0,
	SEEK_DIR_CUR,
	SEEK_DIR_END,
};

/// Abstract stream for reading
class  Deserializer
{
public:
	Deserializer();
	Deserializer(unsigned size);
	virtual ~Deserializer();

	virtual unsigned Read(void* dest, unsigned size) = 0;
	virtual unsigned Seek(unsigned position, SeekDir seekDir = SEEK_DIR_BEGIN) = 0;
	virtual const tinystl::string& GetName() const = 0;
	virtual unsigned GetChecksum();

	unsigned GetPosition() const { return mPosition; }
	unsigned GetSize() const { return mSize; }
	bool IsEof() const { return mPosition >= mSize; }
	int64_t ReadInt64();
	int ReadInt();
	int16_t ReadShort();
	int8_t ReadByte();
	unsigned ReadUInt();
	uint16_t ReadUShort();
	uint8_t ReadUByte();
	bool ReadBool();
	float ReadFloat();
	double ReadDouble();
	float2 ReadVector2();
	float3 ReadVector3();
	float3 ReadPackedVector3(float maxAbsCoord);
	float4 ReadVector4();
	tinystl::string ReadString();
	tinystl::string ReadFileID();
	tinystl::string ReadLine();

protected:
	unsigned mPosition;
	unsigned mSize;
};

/// Abstract stream for writing
class  Serializer
{
public:
	virtual ~Serializer();

	virtual unsigned Write(const void* data, unsigned size) = 0;

	bool WriteInt64(int64_t value);
	bool WriteInt(int value);
	bool WriteShort(int16_t value);
	bool WriteByte(int8_t value);
	bool WriteUInt(unsigned value);
	bool WriteUShort(uint16_t value);
	bool WriteUByte(uint8_t value);
	bool WriteBool(bool value);
	bool WriteFloat(float value);
	bool WriteDouble(double value);
	bool WriteVector2(const float2& value);
	bool WriteVector3(const float3& value);
	bool WritePackedVector3(const float3& value, float maxAbsCoord);
	bool WriteVector4(const float4& value);
	bool WriteString(const tinystl::string& value);
	bool WriteFileID(const tinystl::string& value);
	bool WriteLine(const tinystl::string& value);
};

/// Text / binary file loaded from disk
class File : public Deserializer, public Serializer
{
public:
	File();

	virtual bool Open(const tinystl::string& fileName, FileMode mode, FSRoot root);
	virtual void Close();
	virtual void Flush();

	unsigned Read(void* dest, unsigned size) override;
	unsigned Seek(unsigned position, SeekDir seekDir = SEEK_DIR_BEGIN) override;
	unsigned Write(const void* data, unsigned size) override;

	tinystl::string ReadText();

	virtual unsigned GetChecksum() override;
	virtual const tinystl::string& GetName() const override { return mFileName; }
	virtual FileMode GetMode() const { return mMode; }
	virtual bool IsOpen() const { return pHandle != NULL; }
	virtual bool IsReadOnly() const { return mMode == FileMode::FM_Read || mMode == FileMode::FM_ReadBinary; }
	virtual bool IsWriteOnly() const { return mMode == FileMode::FM_Write || mMode == FileMode::FM_WriteBinary; }
	virtual void* GetHandle() const { return pHandle; }

protected:
	tinystl::string mFileName;
	FileMode mMode;
	FileHandle pHandle;
	unsigned mOffset;
	unsigned mChecksum;
	bool mReadSyncNeeded;
	bool mWriteSyncNeeded;
};

/// Memory area simulating a stream
class  MemoryBuffer : public Deserializer, public Serializer
{
public:
	MemoryBuffer(void* data, unsigned size);
	MemoryBuffer(const void* data, unsigned size);

	const tinystl::string& GetName() const override { return mName; }

	unsigned Read(void* dest, unsigned size) override;
	unsigned Seek(unsigned position, SeekDir seekDir = SEEK_DIR_BEGIN) override;
	unsigned Write(const void* data, unsigned size) override;

	unsigned char* GetData() { return pBuffer; }
	bool IsReadOnly() { return mReadOnly; }

private:
	tinystl::string mName;
	unsigned char* pBuffer;
	bool mReadOnly;
};

/// High level platform independent file system
class FileSystem
{
public:
	static unsigned GetFileSize(FileHandle handle);
	// Allows to modify root paths at runtime
	static void	 SetRootPath(FSRoot root, const tinystl::string& rootPath);
	// Reverts back to App static defined pszRoots[]
	static void	 ClearModifiedRootPaths();
	static unsigned GetLastModifiedTime(const tinystl::string& _fileName);
	// First looks it root exists in m_ModifiedRootPaths
	// otherwise uses App static defined pszRoots[]
	static tinystl::string  FixPath(const tinystl::string& pszFileName, FSRoot root);
	static bool	 FileExists(const tinystl::string& pszFileName, FSRoot root);

	static tinystl::string  GetCurrentDir() { return AddTrailingSlash(_getCurrentDir()); }
	static tinystl::string  GetProgramDir() { return GetPath(_getExePath()); }
	static tinystl::string  GetUserDocumentsDir() { return AddTrailingSlash(_getUserDocumentsDir()); }
	static tinystl::string  GetAppPreferencesDir(const tinystl::string& org, const tinystl::string& app) { return AddTrailingSlash(_getAppPrefsDir(org, app)); }
	static void	 GetFilesWithExtension(const tinystl::string& dir, const tinystl::string& ext, tinystl::vector<tinystl::string>& files);

	static void	 SetCurrentDir(const tinystl::string& path) { _setCurrentDir(path.c_str()); }

	static void	 SplitPath(const tinystl::string& fullPath, tinystl::string* pathName, tinystl::string* fileName, tinystl::string* extension, bool lowercaseExtension = true);
	static tinystl::string  GetPath(const tinystl::string& fullPath);
	static tinystl::string  GetFileName(const tinystl::string& fullPath);
	static tinystl::string  GetExtension(const tinystl::string& fullPath, bool lowercaseExtension = true);
	static tinystl::string  GetFileNameAndExtension(const tinystl::string& fullPath, bool lowercaseExtension = false);
	static tinystl::string  ReplaceExtension(const tinystl::string& fullPath, const tinystl::string& newExtension);
	static tinystl::string  AddTrailingSlash(const tinystl::string& pathName);
	static tinystl::string  RemoveTrailingSlash(const tinystl::string& pathName);
	static tinystl::string  GetParentPath(const tinystl::string& pathName);
	static tinystl::string  GetInternalPath(const tinystl::string& pathName);
	static tinystl::string  GetNativePath(const tinystl::string& pathName);

	static bool	 DirExists(const tinystl::string& pathName);
	static bool	 CreateDir(const tinystl::string& pathName);
	static int	  SystemRun(const tinystl::string& fileName, const tinystl::vector<tinystl::string>& arguments, tinystl::string stdOut = "");
	static bool	 Delete(const tinystl::string& fileName);

private:
	// The following root paths are the ones that were modified at run-time
	static tinystl::string  mModifiedRootPaths[FSRoot::FSR_Count];
	static tinystl::string  mProgramDir;
};
