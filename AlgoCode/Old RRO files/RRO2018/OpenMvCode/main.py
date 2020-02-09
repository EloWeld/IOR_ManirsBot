import sensor, image, time, machine
from pyb import UART
from pyb import LED
uart = UART(3, 9600)
Add = False
Loop = False #True
Debug = False #True
Plus_V_M = -7
Plus_V_B = -7
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 1700)
LED(1).on()
LED(2).on()
LED(3).on()
class RGB():
    def __init__(self):
        self.R = 0
        self.G = 0
        self.B = 0
    def __repr__(self):
        return (self.R, self.G, self.B)
class HSV():
    def __init__(self):
        self.H = 0
        self.S = 0
        self.V = 0
    def __repr__(self):
        return (self.H, self.S, self.V)
class Element():
    def __init__(self):
        self.RGB = RGB()
        self.HSV = HSV()
        self.rect = [0, 0, 0, 0]
        self.ClrName = "?"
        self.ClrCode = 1
def RGBtoHSV(ElX):
    rgbMin = min([ElX.RGB.R, ElX.RGB.G, ElX.RGB.B])
    ElX.HSV.V = max([ElX.RGB.R, ElX.RGB.G, ElX.RGB.B])
    delta = ElX.HSV.V - rgbMin
    if ElX.HSV.V == 0:
        ElX.HSV.S = 0
    else:
        ElX.HSV.S = 100 * delta // ElX.HSV.V
    if (ElX.HSV.S == 0):
        ElX.HSV.H = 0
    else:
        if ElX.HSV.V == ElX.RGB.R:
            ElX.HSV.H = (ElX.RGB.G - ElX.RGB.B) / delta
        elif ElX.HSV.V == ElX.RGB.G:
            ElX.HSV.H = 2 + (ElX.RGB.B - ElX.RGB.R) / delta
        else:
            ElX.HSV.H = 4 + (ElX.RGB.R - ElX.RGB.G) / delta
        ElX.HSV.H = ElX.HSV.H * 60
        ElX.HSV.H = int(round(ElX.HSV.H))
        if ElX.HSV.H < 0:
            ElX.HSV.H += 360
    ElX.HSV.V = int(ElX.HSV.V // 2.55)
    return ElX
def DefineColor(img, Rect):
    Elem = Element()
    Elem.rect = Rect
    for i in range(Rect[0], Rect[2]+Rect[0]):
        for j in range(Rect[1], Rect[3] + Rect[1]):
            Elem.RGB.R += img.get_pixel(i, j)[0]
            Elem.RGB.G += img.get_pixel(i, j)[1]
            Elem.RGB.B += img.get_pixel(i, j)[2]
    tmp = (Rect[2] * Rect[3])
    Elem.RGB.R //= tmp
    Elem.RGB.G //= tmp
    Elem.RGB.B //= tmp
    RGBtoHSV(Elem)
    return Elem
def DefineColorName(ElemX):
    if ElemX.HSV.S > 40 and ElemX.HSV.V > 20:
        if ElemX.HSV.H < 23:
            ElemX.ClrName = "Red"
            ElemX.ClrCode = 1
            return ElemX
        elif ElemX.HSV.H < 75:
            ElemX.ClrName = "Yellow"
            ElemX.ClrCode = 6
            return ElemX
        elif ElemX.HSV.H < 165:
            ElemX.ClrName = "Green"
            ElemX.ClrCode = 2
            return ElemX
        elif ElemX.HSV.H < 270:
            ElemX.ClrName = "Blue"
            ElemX.ClrCode = 3
            return ElemX
        elif ElemX.HSV.H < 360:
            ElemX.ClrName = "Red"
            ElemX.ClrCode = 1
            return ElemX
        else:
            print("ERROR: Color Not Found!")
    else:
        if ElemX.HSV.V < 50:
            ElemX.ClrName = "Black"
            ElemX.ClrCode = 4
            return ElemX
        else:
            ElemX.ClrName = "White"
            ElemX.ClrCode = 5
            return ElemX
def MostBlack(clrs):
    clrPoints = [0, 0, 30, 120, 240, 0]
    clrColors = [999, -999, 999, 999, 999, 999]
    end = [-1, -1, -1, -1, -1, -1]
    min1, min2 = 0, 0
    if clrs[0].HSV.S > clrs[1].HSV.S:
        min1 = 0
        min2 = 1
    else:
        min1 = 1
        min2 = 0
    for i in range(2,6):
        if clrs[i].HSV.S < clrs[min1].HSV.S:
            b = min1
            min1 = i
            if clrs[b].HSV.S < clrs[min2].HSV.S:
                min2 = b
        elif clrs[i].HSV.S < clrs[min2].HSV.S:
            min2 = i
    if clrs[min1].HSV.V > clrs[min2].HSV.V:
        end[0] = min2
        end[1] = min1
    else:
        end[0] = min1
        end[1] = min2
    for i in range(len(clrs)):
        if clrs[i].HSV.S < clrColors[0]:
            clrColors[0] = clrs[i].HSV.S
    for step in range(2, 5):
        for i in range(len(clrs)):
            if not (i in end):
                if abs(clrs[i].HSV.H - clrPoints[step]) < clrColors[step]:
                    clrColors[step] = abs(clrs[i].HSV.H - clrPoints[step])
                    end[step] = i
    for i in range(len(clrs)):
        if not (i in end):
            end[5] = i;
            break;
    for i in range(6):
        clrs[end[i]].ClrName = ("Black", "White", "Yellow", "Green", "Blue", "Red")[i]
        clrs[end[i]].ClrCode = (4, 5, 6, 2, 3, 1)[i]
def main():
    # Делаем фото с камеры
    img = sensor.snapshot()
    img.save("/img.jpg")
    # Создаём массивы прямоугольников по размерам
    b_rects = [[12, 146+Plus_V_B, 26, 34], [50, 149+Plus_V_B, 33, 35], [105, 152+Plus_V_B, 31, 34], \
               [164, 152+Plus_V_B, 34, 32], [219, 151+Plus_V_B, 35, 31], [274, 152+Plus_V_B, 30, 27]]
    m_rects = [[110, 73+Plus_V_M, 9, 7], [124, 74+Plus_V_M, 13, 6], [141, 73+Plus_V_M, 13, 7], \
               [160, 72+Plus_V_M, 10, 8], [176, 71+Plus_V_M, 12, 8], [193, 70+Plus_V_M, 12, 10]]

    # Создаём массивы
    boxes = []
    marks = []
    # Заполняем цветами
    for i in range(6):
        boxes.append(DefineColor(img, b_rects[i]))
    for i in range(6):
        marks.append(DefineColor(img, m_rects[i]))
    # Отказоустойчивасть
    MostBlack(boxes)
    MostBlack(marks)
    if Debug:
        for i in range(6):
            img.draw_rectangle(boxes[i].rect, \
                              (boxes[i].RGB.R, boxes[i].RGB.G, boxes[i].RGB.B), fill=True)
        for i in range(6):
            img.draw_rectangle(marks[i].rect, \
                              (marks[i].RGB.R, marks[i].RGB.G, marks[i].RGB.B), fill=True)
        img.save("/draw_img.jpg")
        print("\nMarkers:	  ", end="")
        for i in range(6):
            print("{0:10}| ".format(marks[i].ClrName + " " + str(marks[i].HSV.H)), end="")
        print("\nBoxes:		", end="")
        for i in range(6):
            print("{0:10}| ".format(boxes[i].ClrName + " " + str(boxes[i].HSV.H)), end="")
        print("\nMarksHSV:	 ", end="")
        for i in range(6):
            print("{0:10}| ".format(str(marks[i].HSV.H) + " " + str(marks[i].HSV.S) + \
                  " " + str(marks[i].HSV.V)), end="")
        print("\nBoxesHSV:	 ", end="")
        for i in range(6):
            print("{0:10}| ".format(str(boxes[i].HSV.H) + " " + str(boxes[i].HSV.S) + \
                  " " + str(boxes[i].HSV.V)), end="")
        print("\nMarksRGB:	 ", end="")
        for i in range(6):
            print("{0:10}| ".format(str(marks[i].RGB.R) + " " + str(marks[i].RGB.G) + \
                  " " + str(marks[i].RGB.B)), end="")
        print("\nBoxesRGB:	 ", end="")
        for i in range(6):
            print("{0:10}| ".format(str(boxes[i].RGB.R) + " " + str(boxes[i].RGB.G) + \
                  " " + str(boxes[i].RGB.B)), end="")
        print('\n');
        print("\nCode Markers: ", end = "")
        for i in range(6):
            print(str(marks[i].ClrCode), end="")
        print("\nCode Boxes:   ", end = "")
        for i in range(6):
            print(str(boxes[i].ClrCode), end="")
        print(end="\n\n")
    # ДОП ЗАДАНИЕ КОТОРЫОЕ НЕ НУЖНО
    path = []
    x = 0
    new_marks = [1, 2, 3, 4 ,5, 6]
    for i in range(6):
        for j in range(6):
            if boxes[j].ClrCode == marks[i].ClrCode:
                x = j
        new_marks[i] = marks[x].ClrCode
    Code = str(boxes[0].ClrCode)+str(boxes[1].ClrCode)+str(boxes[2].ClrCode)+ \
           str(boxes[3].ClrCode)+str(boxes[4].ClrCode)+str(boxes[5].ClrCode)
    MarksCode = str(marks[0].ClrCode) + str(marks[1].ClrCode) + str(marks[2].ClrCode) + \
                       str(marks[3].ClrCode) + str(marks[4].ClrCode) + str(marks[5].ClrCode)
    MarksCodeDop = str(new_marks[0]) + str(new_marks[1]) + str(new_marks[2]) + \
                str(new_marks[3]) + str(new_marks[4]) + str(new_marks[5])
    #if Debug:
        #print("\nDopStuff: ", end = "")
        #print(MarksCodeDop)
    with open("copy_paths/" + MarksCode + "000000.txt", "r") as f:
        for lin in range(719):
            line = f.readline()
            if line[:6] == Code:
                line = line.split()
                path = line[2:]
                if Debug:
                    print(path, end = "L")
    f.close()
    if not Debug:
        if path == [] or Add:
            i_am_smart = False
            uart.write(str(8))
            for i in range(6):
                uart.write(str(marks[i].ClrCode))
            for i in range(6):
                uart.write(str(boxes[i].ClrCode))
        else:
            i_am_smart = True
            uart.write(str(9))
            for i in range(len(path)):
                uart.write(str(path[i][0]))
                uart.write(str(path[i][1]))
            uart.write(str(7))
if Loop:
    while 1:
        main()
else:
    main()
