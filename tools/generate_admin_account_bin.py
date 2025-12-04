import argparse
import struct
from pathlib import Path

HEADER_FORMAT = "<II"
# AccountRecord layout with pragma pack(1):
# id[32], fullName[128], dob[3i], active[uint8],
# accountId[32], username[64], passwordHash[128], role[32], employeeId[32],
# createdAt[7i], lastLogin[7i]
ACCOUNT_FORMAT = "<32s128s3iB32s64s128s32s32s7i7i"
ACCOUNT_SIZE = struct.calcsize(ACCOUNT_FORMAT)


def _pad(text: str, length: int) -> bytes:
    """Encode and truncate/pad a string to a fixed-length null-terminated byte array."""
    return text.encode("utf-8")[:length].ljust(length, b"\x00")


def create_admin_account_bin(
    path: Path,
    username: str = "admin",
    password: str = "123456",
    role: str = "ADMIN",
) -> None:
    record = struct.pack(
        ACCOUNT_FORMAT,
        _pad("A001", 32),  # id
        _pad("Admin", 128),  # fullName
        1990,
        1,
        1,  # dob
        1,  # active
        _pad("AC001", 32),  # accountId
        _pad(username, 64),  # username (key)
        _pad(password, 128),  # passwordHash (plaintext in this app)
        _pad(role, 32),  # role
        _pad("", 32),  # employeeId (optional)
        2024,
        1,
        1,
        0,
        0,
        0,
        0,  # createdAt
        0,
        0,
        0,
        0,
        0,
        0,
        0,  # lastLogin
    )

    header = struct.pack(HEADER_FORMAT, ACCOUNT_SIZE, 1)
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("wb") as f:
        f.write(header)
        f.write(record)
    print(f"Đã tạo tài khoản mẫu: {username}/{password} tại {path}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate admin login data (users.bin).")
    parser.add_argument(
        "--path",
        type=Path,
        default=Path("data/users.bin"),
        help="Đường dẫn file users.bin sẽ được tạo (mặc định: data/users.bin)",
    )
    parser.add_argument(
        "--username",
        default="admin",
        help="Tên đăng nhập muốn tạo (mặc định: admin)",
    )
    parser.add_argument(
        "--password",
        default="123456",
        help="Mật khẩu đăng nhập (mặc định: 123456)",
    )
    parser.add_argument(
        "--role",
        default="ADMIN",
        help="Vai trò, nên để ADMIN để bật toàn quyền (mặc định: ADMIN)",
    )
    args = parser.parse_args()
    create_admin_account_bin(args.path, args.username, args.password, args.role)
