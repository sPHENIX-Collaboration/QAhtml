#include "OfflineQAKSTest.h"

#include <TFile.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile2D.h>
#include <TROOT.h>
#include <TString.h>
#include <TSystem.h>

#include <TClass.h>
#include <TKey.h>

#include <cstdlib>
#include <iostream>

OfflineQAKSTest::OfflineQAKSTest(const std::string &inputfile, const std::string &goodrunfile)
    : m_inputfile(inputfile)
    , m_goodrunfile(goodrunfile)
    , m_inputfileptr(nullptr)
    , m_goodrunfileptr(nullptr)
{
    if (m_inputfile.empty())
    {
        std::cerr << "OfflineQAKSTest: input file is empty" << std::endl;
        exit(1);
    }
    if (m_goodrunfile.empty())
    {
        std::cerr << "OfflineQAKSTest: good run file is empty" << std::endl;
        exit(1);
    }

    m_inputfileptr = TFile::Open(m_inputfile.c_str(), "READ");
    if (!m_inputfileptr)
    {
        std::cerr << "OfflineQAKSTest: could not open file " << m_inputfile << std::endl;
        exit(1);
    }

    m_goodrunfileptr = TFile::Open(m_goodrunfile.c_str(), "READ");
    if (!m_goodrunfileptr)
    {
        std::cerr << "OfflineQAKSTest: could not open file " << m_goodrunfile << std::endl;
        exit(1);
    }

    return;
}

OfflineQAKSTest::~OfflineQAKSTest()
{
    if (m_inputfileptr)
    {
        m_inputfileptr->Close();
        delete m_inputfileptr;
    }
    if (m_goodrunfileptr)
    {
        m_goodrunfileptr->Close();
        delete m_goodrunfileptr;
    }
    return;
}

void OfflineQAKSTest::AddHistogramNames(const std::set<std::string> &names)
{
    for (const auto &name : names)
    {
        // make sure the histogram exists in both files
        if (!m_inputfileptr->Get(name.c_str()))
        {
            std::cerr << "OfflineQAKSTest: histogram " << name << " not found in input file" << std::endl;
            return;
        }
        if (!m_goodrunfileptr->Get(name.c_str()))
        {
            std::cerr << "OfflineQAKSTest: histogram " << name << " not found in good run file" << std::endl;
            return;
        }

        m_histogram_names.insert(name);
    }
    return;
}

TH1D *OfflineQAKSTest::GenKSTestSummary()
{
    if (m_histogram_names.empty())
    {
        std::cerr << "OfflineQAKSTest: no histograms to compare" << std::endl;
        return nullptr;
    }

    TH1D *h = new TH1D("h_SiSeedsKSTestSummary", "SiSeeds KS Test Summary", m_histogram_names.size(), 0.5, m_histogram_names.size() + 0.5);
    h->SetStats(0);
    h->SetMarkerStyle(20);
    h->SetMarkerSize(1.5);
    h->SetMarkerColor(kBlue);

    h->GetXaxis()->SetTitle("");
    h->GetYaxis()->SetTitle("Kolmogorov-Smirnov Result");

    int i = 1;
    for (const auto &name : m_histogram_names)
    {
        double KS_result = 0;
        // figure out the type of histogram
        TKey *key = m_inputfileptr->FindKey(name.c_str());
        if (!key)
        {
            std::cerr << "OfflineQAKSTest: could not find histogram " << name << std::endl;
            return nullptr;
        }

        TClass *cl = gROOT->GetClass(key->GetClassName());
        if (!cl)
        {
            std::cerr << "OfflineQAKSTest: could not get class for histogram " << name << std::endl;
            return nullptr;
        }

        // get the histogram
        if (cl->InheritsFrom("TH1"))
        {
            TH1 *h1 = dynamic_cast<TH1 *>(m_inputfileptr->Get(name.c_str()));
            TH1 *h2 = dynamic_cast<TH1 *>(m_goodrunfileptr->Get(name.c_str()));
            if (!h1 || !h2)
            {
                std::cerr << "OfflineQAKSTest: could not get histograms for " << name << std::endl;
                return nullptr;
            }

            KS_result = OfflineQAKSTest::KSTest(h1, h2, "D");
        }
        else if (cl->InheritsFrom("TH2"))
        {
            TH2 *h1 = dynamic_cast<TH2 *>(m_inputfileptr->Get(name.c_str()));
            TH2 *h2 = dynamic_cast<TH2 *>(m_goodrunfileptr->Get(name.c_str()));
            if (!h1 || !h2)
            {
                std::cerr << "OfflineQAKSTest: could not get histograms for " << name << std::endl;
                return nullptr;
            }

            KS_result = OfflineQAKSTest::KSTest(h1, h2, "D");
        }
        else if (cl->InheritsFrom("TProfile2D"))
        {
            TProfile2D *p1 = dynamic_cast<TProfile2D *>(m_inputfileptr->Get(name.c_str()));
            TProfile2D *p2 = dynamic_cast<TProfile2D *>(m_goodrunfileptr->Get(name.c_str()));
            if (!p1 || !p2)
            {
                std::cerr << "OfflineQAKSTest: could not get histograms for " << name << std::endl;
                return nullptr;
            }

            // convert to TH2
            TH2D *h1 = p1->ProjectionXY();
            TH2D *h2 = p2->ProjectionXY();

            KS_result = OfflineQAKSTest::KSTest(h1, h2, "D");
        }
        else
        {
            std::cerr << "OfflineQAKSTest: unsupported histogram type for " << name << std::endl;
            return nullptr;
        }
        std::cout << "KS result for " << name << ": " << KS_result << std::endl;
        h->SetBinContent(i, KS_result);
        h->GetXaxis()->SetBinLabel(i, name.c_str());
        i++;
    }

    h->GetXaxis()->SetLabelSize(0.03);
    h->LabelsOption("v");

    return h;
}

template <typename T> double OfflineQAKSTest::KSTest(T *h1, T *h2, const char *options)
{
    // make sure the histograms are not null
    if (!h1 || !h2)
    {
        std::cerr << "KSTest: null histogram pointer" << std::endl;
        return -1;
    }

    // make sure the histograms are not empty
    if (h1->GetEntries() == 0 || h2->GetEntries() == 0)
    {
        std::cerr << "KSTest: empty histogram" << std::endl;
        return -1;
    }

    // make sure they are the same type
    if (h1->IsA() != h2->IsA())
    {
        std::cerr << "KSTest: different histogram types" << std::endl;
        return -1;
    }

    return h1->KolmogorovTest(h2, options);
}
