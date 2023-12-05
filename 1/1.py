with open("./1/1.in") as f:
    lines = [l.strip() for l in f.readlines()]

nums = []
for line in lines:
    print(line)
    calibration_nums = []
    for c in line:
        try:
            num = int(c)
        except:
            continue
        calibration_nums.append(num)
    nums.append(calibration_nums[0] * 10 + calibration_nums[-1])

print(nums)
