#pragma once
#include <vector>

class Object;

class ObjectStorage
{
public:
	static ObjectStorage& getInstance()
	{
		static ObjectStorage instance;
		return instance;
	}

	void AddObject(Object* object)
	{
		objects.push_back(object);
	}

	void AddCubeObject(Object* cube)
	{
		cubes.push_back(cube);
	}

	void AddSphereObject(Object* sphere)
	{
		spheres.push_back(sphere);
	}

	void AddPlaneObject(Object* plane)
	{
		planes.push_back(plane);
	}

	std::vector<Object*>& GetObjects()
	{
		return objects;
	}

	std::vector<Object*>& GetCubeObjects()
	{
		return cubes;
	}

	std::vector<Object*>& GetSphereObjects()
	{
		return spheres;
	}

	std::vector<Object*>& GetPlaneObjects()
	{
		return planes;
	}

private:
	std::vector<Object*> objects;
	std::vector<Object*> cubes;
	std::vector<Object*> spheres;
	std::vector<Object*> planes;

	ObjectStorage() {}

public:
	ObjectStorage(ObjectStorage const&) = delete;
	void operator =(ObjectStorage const&) = delete;
};