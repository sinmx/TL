#include <iostream>
#include <string>
#include <list>

using namespace std;

// ����Ĺ۲���(������) -->ѧ��
class  Listenner
{
public:
	// ��ʦ���ˣ��Ҹ���ô��
	virtual void onTeacherComming() = 0;
	// �ɻ���
	virtual void doBadthing() = 0;

};

// ����ı��۲��ߣ�(֪ͨ�ߣ�ӵ�й۲��ߣ�Ŀ��)
class Notifier
{
public:
	// ��ӹ۲��ߵķ���
	virtual void addListenner(Listenner *listenner) = 0;
	// ɾ���۲��ߵķ���
	virtual void delListenner(Listenner *listenner) = 0;
	// ֪ͨ�۲��ߵķ���
	virtual void notify() = 0;
};

// ����Ĺ۲���
class Student : public Listenner
{
public:
	Student(string name, string badthing)
	{
		this->name = name;
		this->badthing = badthing;
	}

	// ��ʦ���ˣ�
	virtual void onTeacherComming()
	{
		cout << "ѧ�� " << name << "���ְ೤������ ֹͣ" << badthing << " ��Ϊд��ҵ" << endl;
	}
	virtual void doBadthing()
	{
		cout << "ѧ�� " << name << " ����" << badthing << endl;
	}
private:
	string name;
	string badthing;
};

// �����֪ͨ��(�೤)
class Monitor : public Notifier
{
public:
	// ��ӹ۲��ߵķ���
	virtual void addListenner(Listenner *listenner)
	{
		this->l_list.push_back(listenner);
	}
	// ɾ���۲��ߵķ���
	virtual void delListenner(Listenner *listenner)
	{

		this->l_list.remove(listenner);

	}
	// ֪ͨ�۲��ߵķ���,�㲥��Ϣ����ÿ��ѧ����������д�Լ���onTeacherComming
	virtual void notify()
	{
		for (list<Listenner*>::iterator it = l_list.begin(); it !=  l_list.end(); it++)
		{
			(*it)->onTeacherComming();
		}
	}
private:
	list<Listenner*> l_list;
};


int main()
{
	Listenner * s1 = new Student("����", "����");
	Listenner * s2 = new Student("����", "���˿�");
	Listenner * s3 = new Student("����", "��ȭ");

	Notifier * bossHLD = new Monitor(); // boss ���ϴ�

	// ������ѧ���б��֪֪ͨ�ߣ�����֪ͨ�߽���֪ͨ
	bossHLD->addListenner(s1);
	bossHLD->addListenner(s2);
	bossHLD->addListenner(s3);

	cout << "�����Һ�壬��ʦû����" << endl;
	s1->doBadthing();
	s2->doBadthing();
	s3->doBadthing();
	cout  << endl;

	cout << "�೤������ʦ���ˣ�֪ͨ���" << endl;
	bossHLD->notify();
	getchar();
	return 0;
}