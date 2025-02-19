#include <vector>
#include <algorithm>  // std::random_shuffle
#include <cstdlib>    // std::rand, std::srand
#include <ctime>      // std::time

// 生成 15-puzzle，确保可解
void InitializePuzzle() {
    std::vector<int> tiles;
    for (int i = 1; i <= 15; i++) {
        tiles.push_back(i);
    }

    // 随机种子
    std::srand(std::time(0));

    // 生成可解的拼图
    do {
        std::random_shuffle(tiles.begin(), tiles.end());
    } while (GetInversionCount(tiles) % 2 != 0);  // 逆序数必须是偶数

    // 填充拼图数组（空位固定在[3][3]）
    int index = 0;
    for (int i = 0; i < BLOCK_COUNT; i++) {
        for (int j = 0; j < BLOCK_COUNT; j++) {
            if (i == BLOCK_COUNT - 1 && j == BLOCK_COUNT - 1) {
                m_iBlockState[i][j] = 0;
                m_spBlock[XYToOneIndex(j, i)] = new CSprite("NULL");
            } else {
                m_iBlockState[i][j] = tiles[index++];
                char* tmpName = CSystem::MakeSpriteName("PictureBlock", m_iBlockState[i][j]);
                m_spBlock[XYToOneIndex(j, i)] = new CSprite(tmpName);
                MoveSpriteToBlock(m_spBlock[XYToOneIndex(j, i)], j, i);
            }
        }
    }
}

// 计算逆序数（Inversion Count）
int GetInversionCount(const std::vector<int>& tiles) {
    int invCount = 0;
    for (size_t i = 0; i < tiles.size(); i++) {
        for (size_t j = i + 1; j < tiles.size(); j++) {
            if (tiles[i] > tiles[j]) invCount++;
        }
    }
    return invCount;
}
