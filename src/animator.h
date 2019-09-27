#pragma once
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/config.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>

struct AnimationNode;

class Animator
{
public:
	Animator(const aiScene* pScene, unsigned int pAnimIndex);
	~Animator();
	void SetAnimationIndex(unsigned int uAnimationIndex);
	void SetAnimationTime(unsigned int millis);
	void SetAnimationSpeed(float fSpeed);
	void SetLooped(bool bLooped);
	unsigned int GetMaxAnimationTime();
	unsigned int GetAnimationTime(); // in milliseconds

private:
	void UpdateAnimation(long lElapsedTime);
	const std::vector<aiMatrix4x4>& GetBoneMatrices(const aiNode* pNode, unsigned int pMeshIndex = 0);
	const aiMatrix4x4& GetGlobalTransform(const aiNode* node) const;

	AnimationNode* CreateNodeTree(aiNode* pNode, AnimationNode* pParent);
	void CalculateGlobalTransform(AnimationNode* pInternalNode);
	void UpdateTransforms(AnimationNode* pNode, const std::vector<aiMatrix4x4>& vTransforms);
	
	typedef std::map<const aiNode*, AnimationNode*> NodeMap;
	NodeMap m_mapNodesByName;
	typedef std::map<const char*, const aiNode*> BoneMap;
	BoneMap m_mapBoneNodesByName;

	bool looped;
	unsigned int m_currentTime;
	float m_speed;
	double m_dLastTime;
	glm::uvec3* m_pLastFramePosition;
	std::vector<aiMatrix4x4> m_vTransforms;
	aiMatrix4x4 m_mIdentityMatrix;
	const aiScene* m_pScene;
	AnimationNode* m_pRootNode;
	unsigned int m_uCurrentAnimationIndex;
	const aiAnimation* m_pCurrentAnimation;
	friend class Mesh;
};