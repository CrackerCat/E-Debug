#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <map>

class SectionManager;
class TrieTreeNode
{
public:
	TrieTreeNode();
	std::vector<TrieTreeNode*> SpecialNodes;
	TrieTreeNode** ChildNodes;

	unsigned int SpecialType;	//һ�����ִ�������
	char* EsigText;		        //һ�����ִ�������
	char* FuncName;		        //��������

	bool IsMatched;             //�Ƿ��Ѿ�ƥ���
};

class TrieTree
{
public:
	TrieTree(SectionManager*);
	~TrieTree();

	//��־,��ӡ�Ӻ������
	void Log_PrintSubFunc();
	//����������
	bool LoadSig(const char* lpMapPath);
	//ִ�к���ƥ��
	char* MatchFunc(unsigned char* CodeSrc);
	//ִ��ģ��ƥ��,����Խ��Խ��
	const char* MatchFunc_Fuzzy(unsigned char* CodeSrc, double& out_score);
private:
	//������ͨ�ڵ�
	TrieTreeNode* AddNode(TrieTreeNode* p, std::string Txt);
	//��������ڵ�
	TrieTreeNode* AddSpecialNode(TrieTreeNode* p, unsigned int type, std::string Txt);

	//����ƥ��������
	bool FastMatch(TrieTreeNode* p, unsigned char*& FuncSrc);
	//����ƥ��������
	bool SlowMatch(unsigned char* FuncSrc, std::string& FuncTxt);

	int SimilarMatch(unsigned char* FuncSrc, std::string& FuncTxt, double& out_RightLen, double& out_TotalLen);

	bool SlowMatch_CmpCallApi(unsigned char* pSrc, std::string IATEAT);
	bool SlowMatch_CmpCall(unsigned char* pSrc, std::string FuncName);
	bool SimilarMatch_CmpCall(unsigned char* pSrc, std::string FuncName, double& out_RightLen, double& out_TotalLen);
public:
	std::vector<char*>  MemAllocSave;
	SectionManager* m_SectionManager;

	//�޸ĺ���������
	bool m_IsSetName;
	bool m_IsAligned;
	bool m_IsAllMem;
	bool m_MatchSubName;
protected:
	bool Insert(std::string& FuncTxt, const std::string& FuncName);

private:
	enum NodeType_t
	{
		NODE_NORMAL = 0,
		NODE_LONGJMP = 1,	       //      -->
		NODE_CALL = 2,	           //      <>
		NODE_JMPAPI = 3,	       //      []
		NODE_CALLAPI = 4,	       //      <[]>
		NODE_CONSTANT = 6,	       //      !!
		NODE_LEFTPASS = 11,        //      ?
		NODE_RIGHTPASS = 12,       //       ?
		NODE_ALLPASS = 13          //      ??
	};
	//���ڵ�
	TrieTreeNode* root;


	//�Ӻ���,�������ƺͺ����ı�һһӳ��
	std::map<std::string, std::string> m_subFunc;
	
	//������,first��������,second��������
	std::vector<std::pair<std::string, std::string>> mVec_MainFunc;
	
	//R����Runtime,����ʱ��¼ʵ�ʵ�ַ��Ӧ����,��Ҫ��ͼһ����ַ����������� ,����һΪʵ���ڴ��ַ,������Ϊ��Ӧ����
	std::map<unsigned int, std::string> m_RFunc;
};