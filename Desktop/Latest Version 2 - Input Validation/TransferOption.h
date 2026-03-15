#ifndef TRANSFEROPTION_H
#define TRANSFEROPTION_H

#include <vector>
#include <string>

using namespace std;

struct Transfer
{
    string college;
    vector<string> degree;
    vector<string> activities;
};

class TransferOption
{
private:
    Transfer transfer;
    vector<Transfer> transferOption;

public:
    TransferOption();

    void setTransfer();
    void addTransferOption();
    void displayTransferOption() const;
};

#endif
