#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

/*
* ���ϴ��б� 12171608 ��ǻ�Ͱ��а� ������
* DB Project ���� : B+ Tree ����
* ���� : 85 / 100
*/

#define  rc_c_ptr reinterpret_cast<char*> // bin���� stream�� ���� �߽��̹Ƿ� ���� �̿� �����͸� ���Ͽ� �а� �� �� char*�� ����ȯ �ؾ���.

class IndexEntry { // ������ �ε���
public:
	int key;
	int bid;
	IndexEntry(int key, int nextLevelBID) {
		this->key = key;
		this->bid = nextLevelBID;
	}
};

class DataEntry { // �������� ������
public:
	int key;
	int value;
	DataEntry(int key, int value) {
		this->key = key;
		this->value = value;
	}
};

class NonLeafNode { // �������, �ε��� ����
public:
	NonLeafNode* parentNode;
	int bid; // �̳���� id
	int nextLevelBID; // �Ʒ� �ܰ迡 �ִ� ����� bid ����
	int depth;
	vector<IndexEntry> indexList;
};

class LeafNode { // �������, ������ ����
public:
	NonLeafNode* parentNode;
	int bid; // �� ����� id
	int nextLeafNodeBID; // ������ �ִ� ���� ����� bid
	int depth;
	vector<DataEntry> dataList;
};

bool IndexEntyCompare(IndexEntry i1, IndexEntry i2) {
	return i1.key < i2.key;
}

bool DataEntyCompare(DataEntry d1, DataEntry d2) {
	return d1.key < d2.key;
}

class BPlusTree {
public:
	fstream fs;
	string binaryFileName;
	int blockSize, rootID, depth; // ����� ����ִ� ����
	int dataNum, blockNum;
	
	void create(string binaryFileName, int blockSize) { // bin���� ����, file header�� ������� block size, 0(���� rootID), 0(���� Depth) ���
		int tmp = 0; // ���� rootID�� depth�� 0�̴�
		fs.open(binaryFileName, ios::out | ios::binary); // ������ ������ ������ open �Լ��� �ڵ����� �������ش�.
		fs.write(rc_c_ptr(&blockSize), sizeof(int));
		fs.write(rc_c_ptr(&tmp), sizeof(int));
		fs.write(rc_c_ptr(&tmp), sizeof(int));
		fs.close();
	}

	// bin ���� �о ���� ������� ��������
	void readBinaryFile(string binaryFileName) {
		this->binaryFileName = binaryFileName;
		fs.open(binaryFileName, ios::in | ios::out | ios::binary);
		fs.read(rc_c_ptr(&blockSize), sizeof(int));
		fs.read(rc_c_ptr(&rootID), sizeof(int));
		fs.read(rc_c_ptr(&depth), sizeof(int));
		fs.seekg(0, std::ios::end);
		int fileLength = fs.tellg();
		blockNum = (fileLength - 12) / blockSize;
		dataNum = ((blockSize - 4) / 8);
		fs.close();
	}

	void insert(int key, int value) {
		readBinaryFile(binaryFileName); // �������� ������
		
		if (rootID == 0) { // rootID�� 0, �� ������ ���ʻ���
			makeLeaf(key, value); // ���� ��Ʈ���� ���������
		}
		else {
			LeafNode leaf = findLeaf(key);
			leaf.dataList.push_back(DataEntry(key, value));
			sort(leaf.dataList.begin(), leaf.dataList.end(), DataEntyCompare);
			if (leaf.dataList.size() <= dataNum) { // ����� �����ٸ�, �׳� block�� ������.
				fs.open(binaryFileName, ios::in | ios::out | ios::binary);
				fs.seekp(12 + ((leaf.bid - 1) * blockSize)); // leaf�� id�� ������ �̵�
				
				//������ ������ �ݺ���
				for (DataEntry dataEntry : leaf.dataList) {
					fs.write(rc_c_ptr(&(dataEntry.key)), sizeof(int));
					fs.write(rc_c_ptr(&(dataEntry.value)), sizeof(int));
				}

				//������ ��ä���� �κ� 0���� ä��� �ݺ���
				for (int i = 1; i <= dataNum - leaf.dataList.size(); i++) {
					int tmp = 0;
					fs.write(rc_c_ptr(&tmp), sizeof(int));
					fs.write(rc_c_ptr(&tmp), sizeof(int));
				}
				fs.write(rc_c_ptr(&(leaf.nextLeafNodeBID)), sizeof(int));
				fs.close();
			}
			else { //leaf�� ����� �ѵ��ʰ��ߴٸ� split �ؾ��Ѵ�.
				splitLeaf(leaf);
			}
		}
		// ���� ������ Header ���� ������Ʈ
		fs.open(binaryFileName, ios::in | ios::out | ios::binary);
		fs.write(rc_c_ptr(&blockSize), sizeof(int));
		fs.write(rc_c_ptr(&rootID), sizeof(int));
		fs.write(rc_c_ptr(&depth), sizeof(int));
		fs.close();
	}

	//���ο� LeafNode �����. ù �� ���� ���������� �� 0 �ֱ�
	void makeLeaf(int key, int value) {
		rootID = 1;
		fs.open(binaryFileName, ios::in | ios::out | ios::binary);
		fs.seekp(12 + ((rootID - 1) * blockSize));
		int tmp = 0;
		fs.write(rc_c_ptr(&key), sizeof(int)); // �� �� key��
		fs.write(rc_c_ptr(&value), sizeof(int)); // �� �� value��
		for (int i = 1; i < dataNum; i++) { // 
			fs.write(rc_c_ptr(&tmp), sizeof(int)); //key�� ���ʷ� 0
			fs.write(rc_c_ptr(&tmp), sizeof(int)); //value�� ���ʷ� 0
		}
		fs.write(rc_c_ptr(&tmp), sizeof(int)); // nextBID ���� 0
		fs.close();
	}

	//���ο� NonLeafNode �����. 1 <= dataCount + 1
	void makeNonLeaf(int leftNodeBlockID, int key, int rightNodeBlockID) {
		rootID = ++blockNum;
		depth++;
		fs.open(binaryFileName, ios::in | ios::out | ios::binary);
		fs.seekp(12 + ((rootID - 1) * blockSize));
		fs.write(rc_c_ptr(&leftNodeBlockID), sizeof(int)); // ���� leafnode bid ��
		fs.write(rc_c_ptr(&key), sizeof(int)); // key ��
		fs.write(rc_c_ptr(&rightNodeBlockID), sizeof(int)); // ���� leafnode bid ��
		int tmp = 0;
		for (int i = 1; i <= dataNum - 1; i++) {
			fs.write(rc_c_ptr(&tmp), sizeof(int));
			fs.write(rc_c_ptr(&tmp), sizeof(int));
		}
		fs.close();
	}

	/*
	������� ã��
	1. ������� �ϰ��(depth != B+Tree�� depth) : 
	���� ��尡 <BID, key, BID ...>�� �����̹Ƿ� BID�� ���ʷ� �а� data������ŭ key, BID ����.
	2. ã���� �ϴ� key�� �����ϴ� ���� ����� key���� �۴ٸ� ���� ����� BID�� �Ѿ.
	(ũ�ų� ���ٸ� ���� key�� bid�� �����ϰ� ���� key�� �Ѿ��.)
	3. �ش� ������ �������� �������� �ݺ�.
	4. ��������� ��� : ���� ����<key, value ... nextLeafBID>�� �����̹Ƿ�, key, value�� data ������ŭ �д´�.
	5. �����͵��� �����ϰ� ���Ͻ�Ų��.
	*/
	LeafNode findLeaf(int key) {
		LeafNode leafNode;
		int curID = rootID;
		int depth = 0;
		NonLeafNode* parNode = NULL;
		while (depth != this->depth) { // ������带 ã���� ���� while�� �ݺ�
			NonLeafNode* curNonLeafNode = new NonLeafNode();
			curNonLeafNode->bid = curID;
			curNonLeafNode->depth = depth;
			curNonLeafNode->parentNode = parNode;
			fs.open(binaryFileName, ios::in | ios::out | ios::binary);
			fs.seekg(12 + ((curID - 1) * blockSize)); // ����ID ����ġ ã��
			
			int tmp_nextBID = 0;
			
			fs.read(rc_c_ptr(&tmp_nextBID), sizeof(int)); // ���� ���� nextLevelBID read
			curNonLeafNode->nextLevelBID = tmp_nextBID;

			for (int i = 1; i <= dataNum; i++) { // ���� data ������ŭ key, value�� ���� �� �� 0�� �ƴҶ��� read
				int key, bid;
				fs.read(rc_c_ptr(&key), sizeof(int));
				fs.read(rc_c_ptr(&bid), sizeof(int));
				if (key == 0) { continue; }
				curNonLeafNode->indexList.push_back(IndexEntry(key, bid));
			}
			fs.close();

			//key ���ԵǴ� ���� BID (key�� ���ʿ� �ִ� BID�� �����Ѵ�.)
			int nextBID = curNonLeafNode->nextLevelBID;
			int count = 0;
			for (IndexEntry indexEntry : curNonLeafNode->indexList) {
				if (key < indexEntry.key) {
					curID = nextBID;
					depth++;
					parNode = curNonLeafNode;
					break;
				}
				else {
					nextBID = indexEntry.bid;
					count++;
				}
				if (count == curNonLeafNode->indexList.size()) {
					curID = nextBID;
					depth++;
					parNode = curNonLeafNode;
					break;
				}
			}
		}
		if (depth == this->depth) {
			fs.open(binaryFileName, ios::in | ios::out | ios::binary);
			fs.seekg(12 + ((curID - 1) * blockSize)); // ����ID ����ġ ã��
			leafNode.bid = curID;
			leafNode.parentNode = parNode;
			leafNode.depth = this->depth;
			for (int i = 1; i <= dataNum; i++) {
				int key, value;
				fs.read(rc_c_ptr(&key), sizeof(int));
				fs.read(rc_c_ptr(&value), sizeof(int));
				if (key == 0) { continue; }
				leafNode.dataList.push_back(DataEntry(key, value));
			}
			int nextLeafNodeBID;
			fs.read(rc_c_ptr(&nextLeafNodeBID), sizeof(int));
			leafNode.nextLeafNodeBID = nextLeafNodeBID;
			fs.close();
		}
		return leafNode;
	}

	//������� ���ø�, ���� ������� ������ ���⿡ ���ο� ������带 �����.
	void splitLeaf(LeafNode leftLeaf) {
		LeafNode rightNewLeaf;
		rightNewLeaf.bid = ++blockNum;
		rightNewLeaf.nextLeafNodeBID = leftLeaf.nextLeafNodeBID;
		leftLeaf.nextLeafNodeBID = rightNewLeaf.bid;

		//���� ���� ������ ��忡 ���� ����� �� ������ data�� �ִ´�.

		int splitSize = (dataNum % 2 == 0) ? dataNum / 2 : dataNum / 2 + 1;
		for (int i = 1; i <= splitSize; i++) {
			DataEntry data = leftLeaf.dataList.back();
			leftLeaf.dataList.pop_back();
			rightNewLeaf.dataList.push_back(data);
		}

		sort(rightNewLeaf.dataList.begin(), rightNewLeaf.dataList.end(), DataEntyCompare);

		/*
		����(����) ��� �ۼ�
		*/
		fs.open(binaryFileName, ios::in | ios::out | ios::binary);
		fs.seekp(12 + ((leftLeaf.bid - 1) * blockSize)); // leaf�� id�� ������ �̵�
		
		//������ ������ �ݺ���
		for (DataEntry dataEntry : leftLeaf.dataList) {
			fs.write(rc_c_ptr(&(dataEntry.key)), sizeof(int));
			fs.write(rc_c_ptr(&(dataEntry.value)), sizeof(int));
		}
		//������ ��ä���� �κ� 0���� ä��� �ݺ���
		for (int i = 1; i <= dataNum - leftLeaf.dataList.size(); i++) {
			int tmp = 0;
			fs.write(rc_c_ptr(&tmp), sizeof(int));
			fs.write(rc_c_ptr(&tmp), sizeof(int));
		}
		fs.write(rc_c_ptr(&(leftLeaf.nextLeafNodeBID)), sizeof(int));
		fs.close();

		/*
		������ �� ��� �ۼ�
		*/
		fs.open(binaryFileName, ios::in | ios::out | ios::binary);
		fs.seekp(12 + ((rightNewLeaf.bid - 1) * blockSize)); // leaf�� id�� ������ �̵�
		
		//������ ������ �ݺ���
		for (DataEntry dataEntry : rightNewLeaf.dataList) {
			fs.write(rc_c_ptr(&(dataEntry.key)), sizeof(int));
			fs.write(rc_c_ptr(&(dataEntry.value)), sizeof(int));
		}
		//������ ��ä���� �κ� 0���� ä��� �ݺ���
		for (int i = 1; i <= dataNum - rightNewLeaf.dataList.size(); i++) {
			int tmp = 0;
			fs.write(rc_c_ptr(&tmp), sizeof(int));
			fs.write(rc_c_ptr(&tmp), sizeof(int));
		}
		fs.write(rc_c_ptr(&(rightNewLeaf.nextLeafNodeBID)), sizeof(int));
		fs.close();

		int keyForNonLeaf = rightNewLeaf.dataList.front().key; // ������ ��Ÿ�� key��
		//���� ���� ��尡 root ��� ���� ���, ���ο� root��带 ������ش�.
		if (leftLeaf.bid == rootID) {
			makeNonLeaf(leftLeaf.bid, keyForNonLeaf, rightNewLeaf.bid);
		}
		else { // �ƴ϶�� ����(����) ����� �θ𿡰� ���� ����
			NonLeafNode* parNode = leftLeaf.parentNode;
			parNode->indexList.push_back(IndexEntry(keyForNonLeaf, rightNewLeaf.bid));
			sort(parNode->indexList.begin(), parNode->indexList.end(), IndexEntyCompare);

			if (parNode->indexList.size() <= dataNum) { // �θ��� ������ �������� �׳� �����.
				fs.open(binaryFileName, ios::in | ios::out | ios::binary);
				fs.seekp(12 + ((parNode->bid - 1) * blockSize));
				
				fs.write(rc_c_ptr(&parNode->nextLevelBID), sizeof(int)); // ���� ������ ���� ������

				for (IndexEntry indexEntry : parNode->indexList) {
					fs.write(rc_c_ptr(&(indexEntry.key)), sizeof(int));
					fs.write(rc_c_ptr(&(indexEntry.bid)), sizeof(int));
				}

				for (int i = 1; i <= dataNum - parNode->indexList.size(); i++) {
					int tmp = 0;
					fs.write(rc_c_ptr(&tmp), sizeof(int));
					fs.write(rc_c_ptr(&tmp), sizeof(int));
				}
				fs.close();
			}
			else { // �θ��� ������ �ʰ� ��, split�� ��� �Ѵ�.
				splitNonLeaf(parNode);
			}
		}
	}

	// ���� ���ø�. ������ ���⿡ �ϳ� �� �����.
	void splitNonLeaf(NonLeafNode* leftNonLeaf) {
		NonLeafNode* rightNonLeaf = new NonLeafNode();
		rightNonLeaf->bid = ++blockNum;
		int splitSize = (dataNum % 2 == 0) ? dataNum / 2 : dataNum / 2 + 1;

		for (int i = 1; i <= splitSize; i++) {
			IndexEntry index = leftNonLeaf->indexList.back();
			leftNonLeaf->indexList.pop_back();
			rightNonLeaf->indexList.push_back(index);
		}

		sort(rightNonLeaf->indexList.begin(), rightNonLeaf->indexList.end(), IndexEntyCompare);

		//����(����) NonLeaf ���� �ۼ�
		fs.open(binaryFileName, ios::in | ios::out | ios::binary);
		fs.seekp(12 + ((leftNonLeaf->bid - 1) * blockSize));
		fs.write(rc_c_ptr(&leftNonLeaf->nextLevelBID), sizeof(int)); // ���� ������ ���� ������
		for (IndexEntry indexEntry : leftNonLeaf->indexList) {
			fs.write(rc_c_ptr(&(indexEntry.key)), sizeof(int));
			fs.write(rc_c_ptr(&(indexEntry.bid)), sizeof(int));
		}
		for (int i = 1; i <= dataNum - leftNonLeaf->indexList.size(); i++) {
			int tmp = 0;
			fs.write(rc_c_ptr(&tmp), sizeof(int));
			fs.write(rc_c_ptr(&tmp), sizeof(int));
		}
		fs.close();

		//���� �� ���� �� ���ʿ� ���� ����ִ� key(�ּҰ�)�� bid�� �ʿ���(�ּҰ��� root�� �ö󰣴�.)
		int keyForNonLeaf = rightNonLeaf->indexList.front().key;
		rightNonLeaf->nextLevelBID = rightNonLeaf->indexList.front().bid;
		rightNonLeaf->indexList.erase(rightNonLeaf->indexList.begin());

		//���� NonLeaf �ۼ�
		fs.open(binaryFileName, ios::in | ios::out | ios::binary);
		fs.seekp(12 + ((rightNonLeaf->bid - 1) * blockSize));
		fs.write(rc_c_ptr(&rightNonLeaf->nextLevelBID), sizeof(int)); // ���� ������ ���� ������
		for (IndexEntry indexEntry : rightNonLeaf->indexList) {
			fs.write(rc_c_ptr(&(indexEntry.key)), sizeof(int));
			fs.write(rc_c_ptr(&(indexEntry.bid)), sizeof(int));
		}
		for (int i = 1; i <= dataNum - rightNonLeaf->indexList.size(); i++) {
			int tmp = 0;
			fs.write(rc_c_ptr(&tmp), sizeof(int));
			fs.write(rc_c_ptr(&tmp), sizeof(int));
		}
		fs.close();
		//��Ʈ�� ���, ���ο� nonLeaf ����(root ����)
		if (leftNonLeaf->bid == rootID) {
			makeNonLeaf(leftNonLeaf->bid, keyForNonLeaf, rightNonLeaf->bid);
		}
		else { // �ƴ϶�� ������ �θ𿡰� ���� ����
			NonLeafNode* parNode = leftNonLeaf->parentNode;
			parNode->indexList.push_back(IndexEntry(keyForNonLeaf, rightNonLeaf->bid));
			sort(parNode->indexList.begin(), parNode->indexList.end(), IndexEntyCompare);

			if (parNode->indexList.size() <= dataNum) { // �θ��� ������ �������� �׳� �����.
				fs.open(binaryFileName, ios::in | ios::out | ios::binary);
				fs.seekp(12 + ((parNode->bid - 1) * blockSize));
				
				fs.write(rc_c_ptr(&parNode->nextLevelBID), sizeof(int)); // ���� ������ ���� ������

				for (IndexEntry indexEntry : parNode->indexList) {
					fs.write(rc_c_ptr(&(indexEntry.key)), sizeof(int));
					fs.write(rc_c_ptr(&(indexEntry.bid)), sizeof(int));
				}

				for (int i = 1; i <= dataNum - parNode->indexList.size(); i++) {
					int tmp = 0;
					fs.write(rc_c_ptr(&tmp), sizeof(int));
					fs.write(rc_c_ptr(&tmp), sizeof(int));
				}
				fs.close();
			}
			else { // �θ��� ������ �ʰ� ��, split�� ��� �Ѵ�.
				splitNonLeaf(parNode);
			}
		}

	}

	//point search �޼���
	void search(int key, string outputFileName) {
		readBinaryFile(binaryFileName);
		LeafNode leaf = findLeaf(key);
		fs.open(binaryFileName, ios::in | ios::out | ios::binary);
		fs.seekg(12 + ((leaf.bid - 1) * blockSize));
		fstream out(outputFileName, ios::app);
		for (int i = 0; i < dataNum; i++) {
			int _key = 0;
			int _value = 0;
			fs.read(rc_c_ptr(&_key), sizeof(int));
			fs.read(rc_c_ptr(&_value), sizeof(int));
			if (_key == key) {
				out << _key << "," << _value << "\n";
			}
		}
		fs.close();
		out.close();
	}

	//range search
	void rangeSearch(int start, int end, string outputFileName) {
		readBinaryFile(binaryFileName); 
		LeafNode leaf = findLeaf(start);
		fs.open(binaryFileName, ios::in | ios::out | ios::binary);
		fs.seekg(12 + ((leaf.bid - 1) * blockSize));
		fstream out(outputFileName, ios::app);
		int _key = 0;
		int _value = 0;
		int idx = 1;
		int nextBID = 0;
		fs.read(rc_c_ptr(&_key), sizeof(int));
		fs.read(rc_c_ptr(&_value), sizeof(int));
		while (_key < start || _key == 0) {
			if (idx == dataNum) {
				idx = 1;
				fs.read(rc_c_ptr(&nextBID), sizeof(int));
				fs.seekg(12 + ((nextBID - 1) * blockSize));
				fs.read(rc_c_ptr(&_key), sizeof(int));
				fs.read(rc_c_ptr(&_value), sizeof(int));
			} else {
				idx++;
				fs.read(rc_c_ptr(&_key), sizeof(int));
				fs.read(rc_c_ptr(&_value), sizeof(int));
			}
		}
		while (_key >= start && _key <= end) {
			out << _key << "," << _value << "\t";
			if (idx == dataNum) {
				idx = 1;
				fs.read(rc_c_ptr(&nextBID), sizeof(int));
				fs.seekg(12 + ((nextBID - 1) * blockSize));
				fs.read(rc_c_ptr(&_key), sizeof(int));
				fs.read(rc_c_ptr(&_value), sizeof(int));
			}
			else {
				idx++;
				fs.read(rc_c_ptr(&_key), sizeof(int));
				fs.read(rc_c_ptr(&_value), sizeof(int));
			}
			if (_key == 0) {
				while (idx < dataNum) {
					idx++;
					fs.read(rc_c_ptr(&_key), sizeof(int));
					fs.read(rc_c_ptr(&_value), sizeof(int));
				}
				idx = 1;
				fs.read(rc_c_ptr(&nextBID), sizeof(int));
				fs.seekg(12 + ((nextBID - 1) * blockSize));
				fs.read(rc_c_ptr(&_key), sizeof(int));
				fs.read(rc_c_ptr(&_value), sizeof(int));
			}
		}
		out << "\n";
		fs.close();
		out.close();
	}

	void print(string outputFileName) {
		readBinaryFile(binaryFileName);
		fs.open(binaryFileName, ios::in | ios::out | ios::binary);
		fs.seekg(12 + ((rootID - 1) * blockSize));
		fstream out(outputFileName, ios::app);
		out << "<0>\n\n";
		if (this->depth == 0) {
			for (int i = 1; i <= dataNum; i++) {
				int _key = 0;
				int _value = 0;
				fs.read(rc_c_ptr(&_key), sizeof(int));
				fs.read(rc_c_ptr(&_value), sizeof(int));
				if (_key == 0) { continue; }
				if (i == 1) { out << _key; }
				else { out << ", " << _key; }
			}
		}
		else {
			int _key = 0;
			int _value = 0;
			int _bid = 0;
			fs.read(rc_c_ptr(&_bid), sizeof(int));
			vector<int> level_one_bid;
			level_one_bid.push_back(_bid);			
			for (int i = 1; i <= dataNum; i++) {
				fs.read(rc_c_ptr(&_key), sizeof(int));
				fs.read(rc_c_ptr(&_bid), sizeof(int));
				if (_key == 0) { continue; }
				if (i == 1) { out << _key; }
				else { out << ", " << _key; }
				level_one_bid.push_back(_bid);
			}
			out << "\n\n<1>\n\n";
			for (int i = 0; i < level_one_bid.size(); i++) {
				fs.seekg(12 + ((level_one_bid[i] - 1) * blockSize));
				for (int j = 1; j <= dataNum; j++) {
					if (this->depth > 1) {
						fs.read(rc_c_ptr(&_bid), sizeof(int));
					}
					fs.read(rc_c_ptr(&_key), sizeof(int));
					fs.read(rc_c_ptr(&_value), sizeof(int));
					if (_key == 0) { continue; }
					if (i == 0 && j == 1) { out << _key; }
					else { out << ", " << _key; }
				}
			}
		}
		fs.close();
		out.close();
	}
};

/*
command line���� �Ű������� �ٷ� �ѱ�� ���� argc�� argv�� ����ϴ°�. 
argc�� �Ű������� ����. argv�� �Ű������� chgr������ ����Ǵ� ����
*/
int main(int argc, char* argv[]) {
	char command = argv[1][0];
	fstream fs;
	BPlusTree bpt;
	switch (command)
	{
	case 'c':// bin ���� ����
		bpt.create(argv[2], stoi(argv[3]));
		break;
	case 'i': // ����
		fs.open(argv[3]);
		bpt.binaryFileName = argv[2];
		if (fs.is_open()) {
			char* input = new char[24];
			while (fs >> input) {
				int key = stoi(strtok(input, ","));
				int value = stoi(strtok(NULL, " "));
				bpt.insert(key, value);
			}
		}
		fs.close();
		break;
	case 's':// Ű�� VALUE�� Ž��
		fs.open(argv[3]);
		bpt.binaryFileName = argv[2];
		if (fs.is_open()) {
			char* input = new char[24];
			while (fs >> input) {
				bpt.search(stoi(input), argv[4]);
			}
		}
		fs.close();
		break;
	case 'r':// ����Ž��
		fs.open(argv[3]);
		bpt.binaryFileName = argv[2];
		if (fs.is_open()) {
			char* input = new char[24];
			while (fs >> input) {
				int start = stoi(strtok(input, ","));
				int end = stoi(strtok(NULL, " "));
				bpt.rangeSearch(start, end, argv[4]);
			}
		}
		break;
	case 'p': // ����Ʈ
		fs.open(argv[3]);
		bpt.binaryFileName = argv[2];
		bpt.print(argv[3]);
		break;
	}
	return 0;
}
