#include <iostream>
#include "immolate.hpp"
#include <thread>
#include <vector>

long filter(Instance inst) {
    long legendaries = 0;
    inst.nextPack(1);
    for (int p = 1; p <= 3; p++) {
        Pack pack = packInfo(inst.nextPack(1));
        if (pack.type == Item::Arcana_Pack) {
            auto packContents = inst.nextArcanaPack(pack.size, 1);
            for (int x = 0; x < pack.size; x++) {
                if (packContents[x] == Item::The_Soul) legendaries++;
            }
        }
        if (pack.type == Item::Spectral_Pack) {
            auto packContents = inst.nextSpectralPack(pack.size, 1);
            for (int x = 0; x < pack.size; x++) {
                if (packContents[x] == Item::The_Soul) legendaries++;
            }
        }
    }
    return legendaries;
};

long filter_perkeo_observatory(Instance inst) {
    if (inst.nextVoucher(1) == Item::Telescope) {
        inst.activateVoucher(Item::Telescope);
        if (inst.nextVoucher(2) != Item::Observatory) return 0;
    } else return 0;
    int antes[5] = {1, 1, 2, 2, 2};
    for (int i = 0; i < 5; i++) {
        Pack pack = packInfo(inst.nextPack(antes[i]));
        std::vector<Item> packContents;
        if (pack.type == Item::Arcana_Pack) {
            packContents = inst.nextArcanaPack(pack.size, antes[i]);
        } else if (pack.type == Item::Spectral_Pack) {
            packContents = inst.nextSpectralPack(pack.size, antes[i]);
        } else continue;
        for (int x = 0; x < pack.size; x++) {
            if (packContents[x] == Item::The_Soul && inst.nextJoker(ItemSource::Soul, antes[i], true).joker == Item::Perkeo) return 1;
        }
    }
    return 0;
}

long filter_negative_tag(Instance inst) {
    //Note: If the score cutoff was passed as a variable, this code could be significantly optimized
    int maxAnte = 20;
    int score = 0;
    for (int i = 2; i <= maxAnte; i++) {
        if (inst.nextTag(i) == Item::Negative_Tag) score++;
    }
    return score;
}

long filter_suas_speedrun(Instance inst) {
    //Mega Spectral Pack in shop with Ankh and Immolate
    inst.nextPack(1); // Eat the Buffoon Pack
    Item pack = inst.nextPack(1);
    if (pack != Item::Mega_Spectral_Pack) return 0;
    //First two cards in shop must be Mr Bones and Merry Andy
    ShopItem item1 = inst.nextShopItem(1);
    ShopItem item2 = inst.nextShopItem(1);
    if ((item1.item != Item::Mr_Bones && item2.item != Item::Mr_Bones) || (item2.item != Item::Merry_Andy && item1.item != Item::Merry_Andy)) return 0;
    //Check for Ankh and Immolate somewhere in the pack
    std::vector<Item> packContents = inst.nextSpectralPack(4, 1);
    bool ankh = false;
    bool immolate = false;
    for (int i = 0; i < 4; i++) {
        if (packContents[i] == Item::Ankh) ankh = true;
        if (packContents[i] == Item::Immolate) immolate = true;
    }
    if (!ankh || !immolate) return 1;
    inst.initLocks(1, false, true);
    Item tag = inst.nextTag(1);
    if (tag != Item::Speed_Tag && tag != Item::Economy_Tag && tag != Item::Coupon_Tag && tag != Item::Foil_Tag && tag != Item::Holographic_Tag && tag != Item::Polychrome_Tag && tag != Item::Negative_Tag) return 2;
    return 3;
}

int main() {
    Search search(filter_suas_speedrun, "9HS1O8", 12, 100000000000);
    search.printDelay = 100000000;
    search.search();
    return 1;
}