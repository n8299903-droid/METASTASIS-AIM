#include <iostream>
#include <vector>
#include <cmath>

// Структура для хранения данных о сущности (игроке/мобе)
struct Entity {
    int id;
    std::string name;
    double x, y, z;
    double distance;
    bool isAlive;
};

class KillAura {
private:
    double range;
    int fov;
    bool raytraceCheck;
    bool weaponOnly;

    // Расчет угла атаки (Rotation)
    void calculateRotations(double targetX, double targetY, double targetZ, double playerX, double playerY, double playerZ, float &yaw, float &pitch) {
        double diffX = targetX - playerX;
        double diffY = targetY - playerY;
        double diffZ = targetZ - playerZ;
        double dist = sqrt(diffX * diffX + diffZ * diffZ);
        
        yaw = (float)(atan2(diffZ, diffX) * 180.0 / M_PI) - 90.0f;
        pitch = (float)(-(atan2(diffY, dist) * 180.0 / M_PI));
    }

public:
    KillAura(double r, int f, bool ray, bool weapon) 
        : range(r), fov(f), raytraceCheck(ray), weaponOnly(weapon) {}

    // Основной метод тика (вызывается каждый кадр/тик игрока)
    void onTick(double playerX, double playerY, double playerZ, float playerYaw, float playerPitch, const std::vector<Entity>& entities) {
        Entity* bestTarget = nullptr;
        double minDistance = range + 1.0;

        for (const auto& entity : entities) {
            if (!entity.isAlive) continue;
            if (entity.distance > range) continue;

            // Проверка на дистанцию и выбор ближайшего
            if (entity.distance < minDistance) {
                minDistance = entity.distance;
                bestTarget = const_cast<Entity*>(&entity);
            }
        }

        // Если цель найдена — выполняем удар с учетом задержки кулдауна (1.9+ механика)
        if (bestTarget != nullptr) {
            float targetYaw, targetPitch;
            calculateRotations(bestTarget->x, bestTarget->y, bestTarget->z, playerX, playerY, playerZ, targetYaw, targetPitch);

            // Отправка пакетов поворота головы и атаки (пример логики для Fabric Mixin)
            attackTarget(*bestTarget, targetYaw, targetPitch);
        }
    }

    void attackTarget(const Entity& target, float yaw, float pitch) {
        std::cout << "[Vulcan/FunTime Bypass] Атака цели: " << target.name 
                  << " на дистанции " << target.distance 
                  << " | Углы: Yaw=" << yaw << ", Pitch=" << pitch << std::endl;
        // Здесь вызывается packet об ударе CInteractPacket / CPlayerPacket
    }
};

int main() {
    KillAura killAura(3.6, 360, true, true);
    
    // Симуляция данных с сервера
    std::vector<Entity> worldEntities = {
        {1, "Player_Enemy", 2.5, 0.0, 1.2, 2.8, true},
        {2, "Dummy_Mob", 5.0, 0.0, 4.0, 5.2, true}
    };

    // Симуляция игрока
    killAura.onTick(0.0, 0.0, 0.0, 0.0f, 0.0f, worldEntities);

    return 0;
}