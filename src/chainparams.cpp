// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Copyright (c) 2021 The DiamondHands Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>
#include <memory>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Tesla buys 1.5 billion in bitcoin plans to accept it as payment";
    const CScript genesisOutputScript = CScript() << ParseHex("04c4d294bbe76be6ee31ef5ef188adf360aef88f3b11c2dda496e6f9e6ab3e946be98908bcd628d613c77c3a0f33bd00fabe9ef9422fcd9aa414793a2a2ae04fc6") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 200000;
        consensus.BIP16Height = 0; 
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 0; 
        consensus.BIP66Height = 0; 
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); 
        consensus.nPowTargetTimespan =  10 * 60;
        consensus.nPowTargetSpacing = 5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 2; // 75% of 2
        consensus.nMinerConfirmationWindow = 8; // nPowTargetTimespan / nPowTargetSpacing * 4
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; 
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; 

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE; 
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT; 

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE; 
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT; 

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000200020");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x79def583f5688f37cde270e17d0ea46c4581d9f0d39fe89206b4d03b5deaa3ae"); 

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x6e;
        pchMessageStart[1] = 0xa7;
        pchMessageStart[2] = 0xbe;
        pchMessageStart[3] = 0xfc;
        nDefaultPort = 29032;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1612853125, 1716116, 0x1e0ffff0, 1, 10000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x79def583f5688f37cde270e17d0ea46c4581d9f0d39fe89206b4d03b5deaa3ae"));
        assert(genesis.hashMerkleRoot == uint256S("0x39183c44194ff0a4da9ab79a3304db2d52a6654cdc3eb46a1552f957a0276fb9"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.emplace_back("node1.walletbuilders.com");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,1);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,0);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "dmdh";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
                {  0, uint256S("0x79def583f5688f37cde270e17d0ea46c4581d9f0d39fe89206b4d03b5deaa3ae")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 79def583f5688f37cde270e17d0ea46c4581d9f0d39fe89206b4d03b5deaa3ae (height 0).
            1612853125, // * UNIX timestamp of last known number of transactions
            0,  // * total number of transactions between genesis and that timestamp
                    //   (the tx=... number in the SetBestChain debug.log lines)
            0     // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 200000;
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on regtest
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 0; 
        consensus.BIP66Height = 0; 
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan =  10 * 60;
        consensus.nPowTargetSpacing = 5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 2; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; 
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; 

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE; 
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT; 

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE; 
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT; 

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000200020");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x99e1927d0a50a4ec1a1ee80c1cfe3365613277d4b7028476377fc2c95380dca2"); 

        pchMessageStart[0] = 0x62;
        pchMessageStart[1] = 0x8a;
        pchMessageStart[2] = 0xf3;
        pchMessageStart[3] = 0x0d;
        nDefaultPort = 39032;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1612853663, 1059261, 0x1e0ffff0, 1, 10000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x99e1927d0a50a4ec1a1ee80c1cfe3365613277d4b7028476377fc2c95380dca2"));
        assert(genesis.hashMerkleRoot == uint256S("0x39183c44194ff0a4da9ab79a3304db2d52a6654cdc3eb46a1552f957a0276fb9"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,41);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,40);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,168);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tdmdh";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("99e1927d0a50a4ec1a1ee80c1cfe3365613277d4b7028476377fc2c95380dca2")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 99e1927d0a50a4ec1a1ee80c1cfe3365613277d4b7028476377fc2c95380dca2 (height 0)
            1612853663,
            0,
            0
        };

    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on regtest
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 2; // 75% for testchains
        consensus.nMinerConfirmationWindow = 0; // Faster than normal for regtest (0 instead of 2)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x1f;
        pchMessageStart[1] = 0xa3;
        pchMessageStart[2] = 0xc1;
        pchMessageStart[3] = 0x13;
        nDefaultPort = 19444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1612854012, 881548, 0x207fffff, 1, 10000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xe74ada6e8a31bc439d28b9a8670134dc3adf5182c90e467acf82dc90256f7a5d"));
        assert(genesis.hashMerkleRoot == uint256S("0x39183c44194ff0a4da9ab79a3304db2d52a6654cdc3eb46a1552f957a0276fb9"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true; 

        checkpointData = {
            {
                {0, uint256S("e74ada6e8a31bc439d28b9a8670134dc3adf5182c90e467acf82dc90256f7a5d")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,41);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,40);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,168);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "rdmdh";
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
