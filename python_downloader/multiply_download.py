# -*- coding: utf-8 -*-
##加入环境变量方便找到module
import sys
import os
rootPath = 'F:\Python2.7\Lib\lib-tk'
sys.path.append(rootPath)
from ScrolledText import ScrolledText
from tkFileDialog import askdirectory
from tkMessageBox import askyesno
from PIL import Image
import Tkinter as tk
import threading
import socket
import urllib
import time
import re

socket.setdefaulttimeout(5)

class Crawler_plusplus():
    def __init__(self):
        self.rt = tk.Tk()#创建窗口
        self.rt.title('爬取百度图片并进行图片预处理')#窗口标题
        self.menu = tk.Menu(self.rt)#在根窗口里创建一个菜单
        self.rt.config(menu=self.menu)#把新创建菜单定为根窗口的菜单
        self.aboutmenu =tk.Menu(self.menu)#在根窗口菜单里创建一个菜单
        self.picturemenu =tk.Menu(self.menu)#在根窗口菜单里创建一个菜单
        self.aboutmenu.add_command(label='help',command=self.help)#菜单里的菜单增加“help”命令
        self.menu.add_cascade(label='About', menu=self.aboutmenu)#把菜单里的菜单命名为about
        self.menu.add_cascade(label='Picture Proecessing', menu=self.picturemenu)
        self.picturemenu.add_command(label='Start Proecessing', command=self.Picture_Processing)
        self.menu.add_command(label='refresh', command=self.refresh)
        self.menu.add_command(label='Exit', command=self.close)
        self.frame1 = tk.Frame(self.rt,height=260,width=520)#左边模块，负责爬取百度图片
        self.middle = tk.Frame(self.rt,height=260,width=120)#右边模块，用来提示错误信息
        self.lframe1 = tk.LabelFrame(self.frame1,height=60,width=520,text='Save path')
        self.lframe2 = tk.LabelFrame(self.frame1,height=200,width=520,text='Download')
        self.b1 = tk.Button(self.lframe1,text='Choose',width = 8,height=1 , command=self.get_directory1)
        self.b2 = tk.Button(self.lframe1, text='Confirm', width=10, height=1, command=self.confirm_e)
        self.b3 = tk.Button(self.lframe1, text='Cancel', width=10, height=1, command=self.deconfirm_e,state='disabled')
        self.a1 = tk.Button(self.lframe2,text='Get the picture',width=10,height=1,command=self.start_cr,state='disabled')
        self.e1 = tk.Entry(self.lframe1,takefocus=True)
        self.e2 = tk.Entry(self.lframe2,state='disabled')
        self.alo = tk.Label(self.rt, text='Note:')#中间模块的内容
        self.ala = tk.Label(self.rt, text='',foreground='red')#中间模块的错误提示，内容现在是空的
        self.l1 = tk.Label(self.lframe2, text='Key words:',relief='flat')
        self.l2 = tk.Label(self.lframe2, text='Download progress:',relief='groove')
        self.s = ScrolledText(self.lframe2,height=8,width=40,state = 'disabled',wrap='char',borderwidth=0)
        self.frame1.grid(column=0,row=0,rowspan=2,columnspan=1)
        self.lframe1.grid(row=0,column=1)
        self.b1.place(x=45,y=15,anchor='center')
        self.e1.place(x=280,y=15,anchor='e')
        self.b2.place(x=400,y=15,anchor='e')
        self.b3.place(x=480,y=15,anchor='e')

        self.lframe2.grid(row=1,column=1)
        self.l1.place(x=130,y=15,anchor='e')
        self.e2.place(x=280,y=15,anchor='e')
        self.a1.place(x=400,y=15,anchor='e')
        self.l2.place(x=15,y=45,anchor='w')
        self.s.place(x=15,y=115,anchor='w')

        self.middle.grid(column=1,row=0)
        self.ala.place(x=580, y=140, anchor='center')
        self.alo.place(x=580, y=120, anchor='center')

    #图形处理界面
    def Picture_Processing(self):
        top = tk.Toplevel()
        top.title('Picture Processing')
        top.geometry('520x260')
        self.frame2 = tk.Frame(top,height=260,width=520)#框架 用来批量处理图片
        self.lframe3 = tk.LabelFrame(self.frame2,height=140,width=520,text='图片预处理')
        self.lframe4 = tk.LabelFrame(self.frame2, height=60, width=520, text='保存图片集')
        self.lframe5 = tk.LabelFrame(self.frame2,height=60,width=520,text='预处理图片路径选择')
        self.b4 = tk.Button(self.lframe5, text='选择图片目录', width=12, height=1, command=self.get_directory5)
        self.b5 = tk.Button(self.lframe5, text='确定该目录', width=10, height=1, command=self.confirm_e1)
        self.b6 = tk.Button(self.lframe5, text='取消确定', width=10, height=1, command=self.deconfirm_e1, state='disabled')     
        self.a2 = tk.Button(self.lframe3,text='图片处理启动',width=10,height=1,command=self.start_pi,state='disabled')
        self.ad = tk.Button(self.lframe4,text='选择保存文件夹',width=14,height=1,command=self.get_directory4,state='disabled')
        self.a3 = tk.Button(self.lframe4,text='确定该目录',width=10,height=0,command=self.confirm_e2,state='disabled')
        self.a4 = tk.Button(self.lframe4,text='取消确定',width=10,height=0,command=self.deconfirm_e2,state='disabled')
        self.a5 = tk.Button(self.lframe3, text='默认设置', width=10, height=1, command=self.reset, state='disabled')
        self.e3 = tk.Entry(self.lframe5,takefocus=True)
        self.e4 = tk.Entry(self.lframe4,takefocus=True,state='disabled')
        self.alb = tk.Label(self.lframe5, text='',foreground='red')
        self.l3 = tk.Label(self.lframe3, text='处理进度:',relief='groove')
        self.xlen = tk.IntVar()
        self.xlen.set(128)
        self.ylen = tk.IntVar()
        self.ylen.set(128)
        self.xl = tk.Entry(self.lframe3,state='readonly',width=4,textvariable=self.xlen,invcmd=self.alarm)
        self.yl = tk.Entry(self.lframe3, state='readonly',width=4,textvariable=self.ylen,invcmd=self.alarm)
        self.xv = tk.Label(self.lframe3,text='输出图宽:')
        self.yv = tk.Label(self.lframe3,text='输出图高:')
        self.note1=tk.Label(self.lframe3,text='建议输出\n128*128')
        self.s1 = ScrolledText(self.lframe3,height=6,width=40,state = 'disabled',borderwidth=0)
        self.color = tk.IntVar()
        self.color.set(0)
        self.rb1 = tk.Radiobutton(self.lframe3,text='灰色',variable=self.color,value=0,state='disabled')
        self.rb2 = tk.Radiobutton(self.lframe3,text='彩色',variable=self.color,value=1,state='disabled')
        self.cls = tk.Label(self.lframe3,text='输出图片颜色')
        self.type =tk.Label(self.lframe3,text='输出图片格式')
        self.type.place(x=75,y=5,anchor='w')
        self.ty = tk.IntVar()
        self.ty.set(0)
        self.rb3 = tk.Radiobutton(self.lframe3, text='jpg', variable=self.ty, value=0, state='disabled')
        self.rb4 = tk.Radiobutton(self.lframe3, text='png', variable=self.ty, value=1, state='disabled')
        self.rb5 = tk.Radiobutton(self.lframe3, text='jpeg', variable=self.ty, value=2, state='disabled')
        self.rb3.place(x=150, y=5, anchor='w')
        self.rb4.place(x=200, y=5, anchor='w')
        self.rb5.place(x=250, y=5, anchor='w')
        self.a3.place(x=400, y=45, anchor='e')
        self.a4.place(x=480, y=45, anchor='e')
        self.frame2.grid(column=2,row=0,rowspan=3, columnspan=1)
        self.lframe5.grid(column=0, row=0)
        self.b4.place(x=80, y=15, anchor='center')
        self.e3.place(x=280, y=15, anchor='e')
        self.b5.place(x=400, y=15, anchor='e')
        self.b6.place(x=480, y=15, anchor='e')
        self.alb.place(x=550, y=15, anchor='e')

        self.lframe4.grid(column=0, row=1)
        self.ad.place(x=80, y=15, anchor='center')
        self.e4.place(x=280, y=15, anchor='e')
        self.a3.place(x=400, y=15, anchor='e')
        self.a4.place(x=480, y=15, anchor='e')

        self.lframe3.grid(column=0, row=2)
        self.l3.place(x=15, y=15, anchor='w')
        self.s1.place(x=15, y=70, anchor='w')
        self.xv.place(x=330, y=30, anchor='w')
        self.xl.place(x=390, y=30, anchor='w')
        self.yv.place(x=330, y=60, anchor='w')
        self.yl.place(x=390, y=60, anchor='w')
        self.note1.place(x=420, y=45, anchor='w')
        self.rb1.place(x=380, y=5, anchor='w')
        self.rb2.place(x=440, y=5, anchor='w')
        self.cls.place(x=305, y=5, anchor='w')
        self.a2.place(x=480, y=90, anchor='e')
        self.a5.place(x=400,y=90,anchor='e')
    #打开readme.txt文件查看帮助
    def help(self):
        os.system('notepad.exe readme.txt')

    #程序功能无法使用时刷新，重新启动该脚本
    def refresh(self):
        self.quit()
        os.system('python root.py')

    #运行程序，打开窗口
    def run(self):
        self.rt.mainloop()

    #爬图前确认图片关键字是否存在
    def start_cr(self):
        word = self.e2.get()
        if word != '':
            st = askyesno(title='确认开始',message='确定开始下载图片?这会有一段时间的卡顿,中途不要退出页面')
            if st:
                self.start_in()#图片关键字存在，开始爬图
        else:
            self.s.config(state='normal')
            self.s.delete(1.0,'end')
            self.s.insert('end', '请先输入图片关键字(如:周杰伦)')
            self.s.config(state='disabled')

    def close(self):
        cl = askyesno(title='确认关闭',message='确定要关闭吗？')
        if cl:
            self.quit()

    def quit(self):
        self.rt.destroy()

    def confirm_e(self):
        path = self.e1.get()
        if path == '':
            self.ala.config(text='请先选择图片保存目录!')
        elif os.path.exists(path) is False:
            self.ala.config(text='该路径不存在!')
        else:
            self.ala.config(text='')
            self.e1.config(state='disabled')
            self.e2.config(state='normal')
            self.b1.config(state='disabled')
            self.b2.config(state='disabled')
            self.b3.config(state='normal')
            self.a1.config(state='normal')

    def confirm_e1(self):
        path = self.e3.get()
        if path == '':
            self.ala.config(text='请先选择图片目录!')
        elif os.path.exists(path) is False:
            self.ala.config(text='该路径不存在!')
        else:
            self.ala.config(text='')
            self.b4.config(state='disabled')
            self.b5.config(state='disabled')
            self.e3.config(state='disabled')
            self.e4.config(state='normal')
            self.b6.config(state='normal')
            self.ad.config(state='normal')
            self.a3.config(state='normal')

    def confirm_e2(self):
        path = self.e4.get()
        if path == '':
            self.ala.config(text='请先选择图片保存目录!')
        elif os.path.exists(path) is False:
            self.ala.config(text='该路径不存在!')
        else:
            self.ala.config(text='')
            self.b6.config(state='disabled')
            self.ad.config(state='disabled')
            self.e4.config(state='disabled')
            self.a3.config(state='disabled')
            self.a4.config(state='normal')
            self.rb1.config(state='normal')
            self.rb2.config(state='normal')
            self.rb3.config(state='normal')
            self.rb4.config(state='normal')
            self.rb5.config(state='normal')
            self.xl.config(state='normal')
            self.yl.config(state='normal')
            self.a2.config(state='normal')
            self.a5.config(state='normal')

    def deconfirm_e(self):
        self.e1.config(state='normal')
        self.e2.config(state='disabled')
        self.b1.config(state='normal')
        self.b2.config(state='normal')
        self.b3.config(state='disabled')
        self.a1.config(state='disabled')
        self.a3.config(state='disabled')
        self.s.config(state='disabled')

    def deconfirm_e1(self):
        self.b4.config(state='normal')
        self.b5.config(state='normal')
        self.e3.config(state='normal')
        self.e4.config(state='disabled')
        self.b6.config(state='disabled')
        self.ad.config(state='disabled')
        self.a3.config(state='disabled')

    def deconfirm_e2(self):
        self.b6.config(state='normal')
        self.ad.config(state='normal')
        self.e4.config(state='normal')
        self.a3.config(state='normal')
        self.a4.config(state='disabled')
        self.b6.config(state='normal')
        self.ad.config(state='normal')
        self.e4.config(state='normal')
        self.a3.config(state='normal')
        self.a4.config(state='disabled')
        self.rb1.config(state='disabled')
        self.rb2.config(state='disabled')
        self.rb3.config(state='disabled')
        self.rb4.config(state='disabled')
        self.rb5.config(state='disabled')
        self.xl.config(state='disabled')
        self.yl.config(state='disabled')
        self.a2.config(state='disabled')
        self.a5.config(state='disabled')

    def reset(self):
        self.xlen.set(128)
        self.ylen.set(128)
        self.color.set(0)
        self.ty.set(0)
        self.a2.config(state='normal')

    #开始收集'存储地址'和'图片关键字'并开始爬图
    def start_in(self):
        word = self.e2.get()
        self.a1.config(state='disabled')
        self.e2.config(state='disabled')
        self.s.config(state='normal')
        self.b1.config(state='disabled')
        self.b2.config(state='disabled')
        self.b3.config(state='disabled')
        path = self.e1.get()
        self.s.delete(1.0, 'end')
        ins = word.encode('utf-8')
        self.s.insert('end', '正在百度搜索：' + ins)
        self.get_images(word, path)#这是爬图的主程序

    #格式错误反馈
    def alarm(self):
        self.ala.config(text='输入格式有误!')
        #self.a.delete(0,'end')

    #读取网页url
    def readurl(self,url):
        page = urllib.urlopen(url)
        htm = page.read()
        return htm

    #下载网络图片
    #imgurl：单张网络图片的url地址
    #path：图片存储路径
    #x：图片编号，同时也是图片名
    #lastname：网络图片的后缀名
    def downloadimg(self,imgurl,path,x,lastname):
        error_time = 0
        while True:
            time.sleep(1)
            try:
                try:
                    urllib.urlretrieve(imgurl, path + '/%s' % x + lastname)  # 从地址上下载单张图片到指定存储位置上
                    #self.s.insert('insert', '\n第%s张图片下载成功' % x)
                except socket.timeout:  # 防止超时卡死，重新下载
                    self.s.insert('insert', '\nReloading...')
                    count = 1  # 卡死计数
                    while count <= 2:  # 重新下载图片两次
                        try:
                            urllib.urlretrieve(imgurl, path + '/%s' % x + lastname)
                            time.sleep(0.001)
                            self.s.insert('insert', '\n第%s张图片下载成功' % x)
                            break
                        except socket.timeout:
                            err_info = 'Reloading for %d time' % count if count == 1 else 'Reloading for %d times' % count
                            self.s.insert('insert', '\n' + err_info)
                            count = count + 1
                    if count > 2:  # 卡死次数过多
                        # 删除错误文件
                        os.remove(path + '/%s' % x + lastname)

                imgname = path + '/%s' % x + lastname
                size = os.path.getsize(imgname) / float(1024)  # 获取图片大小
                # size单位为kb
                if size < 20:  # 删除小于20kb的图片
                    os.remove(path + '/%s' % x + lastname)
                    self.s.insert('insert', '\n第%s张图片大小太小,跳过' % x)
            except:
                error_time += 1
                if error_time == 100:
                    print ('your network is little bad')
                    time.sleep(60)
                if error_time == 101:
                    print ('your network is broken')
                    break
                continue
            break
            
    #获取图片地址列表
    #jpgre：jpg图片地址的正则表达式形式
    #pngre和jpegre同上
    #htm：网页源代码
    def get_imglist(self,jpgre,pngre,jpegre,htm):
        jpglist = re.findall(jpgre, htm) #在htm里寻找所有jpg图片的源代码，并做成一个列表
        pnglist = re.findall(pngre, htm)
        jpeglist = re.findall(jpegre, htm)
        piclist = [jpglist, pnglist, jpeglist]
        return piclist #piclist是一个三元列表，每一个元素也是一个列表

    #获得文件夹目录
    def get_directory1(self):
        savepath = askdirectory()
        self.e1.delete(0, 'end')
        self.e1.insert('insert',savepath)
    def get_directory5(self):
        savepath = askdirectory()
        self.e3.delete(0, 'end')
        self.e3.insert('insert',savepath)
    def get_directory4(self):
        savepath = askdirectory()
        self.e4.delete(0,'end')
        self.e4.insert('insert',savepath)

    #爬取图片主程序
    #word：图片关键字
    #path：图片存储路径
    def get_images(self,word,path):
        var1 = word.encode('utf-8') #get()函数得到的字符串不是utf-8编码，需要转到utf-8
        search = urllib.quote(var1) #用urllib的网页格式转码utf-8格式的图片关键字
        pn = 0 # pn = 20*(页数+1)
        err_num = 0 #错误计数，连续十个网址下载出现错误时就终止爬图
        x = 1 #图片编号
        jpg = 'objURL":"(.+?.jpg)",'    #百度图片的URL地址的正则表达式
        jpgre = re.compile(jpg)         #编译正则表达式，用于寻找网页源代码里的图片地址
        jpeg = 'objURL":"(.+?.jpeg)",'
        jpegre = re.compile(jpeg)
        png = 'objURL":"(.+?.png)",'
        pngre = re.compile(png)
        self.s.insert('insert','\n--------开始爬图--------')
        while True:
            url = 'http://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word='+search + '&pn=' + str(pn) + '&gsm=3c&ct=&ic=0&lm=-1&width=0&height=0'
            try:
                self.s.insert('insert', '\n正在读取源代码...')
                htm = self.readurl(url) #从网页地址里读取网页源代码
                piclist = self.get_imglist(jpgre,pngre,jpegre,htm) #在网页文件里寻找编译后的URL地址，并返回一个列表
                namelist = ['.jpg','.png','.jpeg'] #后缀名列表，百度图片的后缀名一般只有三种：jpg,png,jpeg
                n = 0
                m = 0
                self.s.insert('insert','\n--------------')
                self.s.insert('insert','\n源代码读取完毕')
                self.s.insert('insert','\n--------------')
                for list in piclist: #注意：piclist是一个三元列表，每一个元素也是一个列表
                    lastname = namelist[n]
                    n = n + 1
                    for imgurl in list: #list存储着许多图片的URL地址，每个imgurl是单张图片的url地址
                        dlth = threading.Thread(target=self.downloadimg, args=(imgurl, path, x, lastname,)) #把下载单张图片交给子进程去完成，提供进程需要执行的目标函数和函数参数
                        dlth.setDaemon(True) #dlth这三行缺一不可
                        dlth.start() #开始执行进程
                        self.s.insert('insert', '\n第%s张图片下载成功' % x)
                        x = x + 1
                        #开始下载下一张图片
                err_num = 0 #一张网页的下载顺利完成，错误代码归零
            except:
                self.s.insert('insert','\n...结束本页内容下载 继续')
                #跳过该页面下载下一页
            pn = pn + 20 #pn = 20*(页数+1)
            err_num = err_num + 1
            if pn > 1960 :#图片关键字只会存100页的图片，所以pn>1960时不可能查到图
                self.s.insert('insert','\n...图片全部下载完毕')
                break
            elif err_num > 10:
                self.s.insert('insert','\n...搜索不到更多图片，下载完毕')
                break
            elif x >= 1024:
                self.s.insert('insert','\n...图片下载已达到上限')
                break
            self.s.insert('insert','\n--------加载下一页--------')

    #以下为图片预处理部分
    def start_pi(self):
        st = askyesno(title='确认开始', message='确定开始批处理图片?这会有一段时间的卡顿，但是可以去存储的文件夹拿到处理好的图')
        if st:
            reload(sys)
            sys.setdefaultencoding('utf8')
            self.start_pic()

    def start_pic(self):
        xLen = self.xl.get()
        yLen = self.yl.get()
        if xLen.isdigit() is True and yLen.isdigit() is True:
            self.ala.config(text='')
            self.a2.config(state='disabled')
            self.s1.config(state='normal')
            self.s1.insert('end', '\nxLen:'+str(xLen))
            self.s1.insert('end', '\nyLen:'+str(yLen))
            picpath = self.e3.get()
            savepath = self.e4.get()
            self.s1.insert('end', '\npicpath:'+picpath)
            self.s1.insert('end', '\nsavepath:'+savepath)
            pictypelist = ['LA' , 'RGB']
            pictype = pictypelist[self.color.get()]
            self.s1.insert('end', '\npictype:'+pictype)
            lastnamelist = ['.jpg','.png','.jpeg']
            lastname = lastnamelist[self.ty.get()]
            self.s1.insert('end', '\nlastname:'+lastname)
            self.pre_main(picpath,savepath,pictype,xLen,yLen,lastname)
        else:
            self.ala.config(text='长宽输入格式有误!')

    def getNameList(self,picpath):
        if os.path.exists(picpath) is False:
            self.ala.config(text='该路径不存在!')
        else:
            self.ala.config(text='')
            files = os.listdir(picpath)
            return files
    def picdeal(self,name,picpath,savepath,pictype,xLen,yLen,lastname):
        try:
            names = name.split('.')[0]
            img = Image.open(picpath+'/'+name).convert(pictype).resize((int(xLen), int(yLen)))
            img.save(savepath +'/%s'%names + lastname)
        except socket.timeout:
            count = 1
            while count <= 2:
                try:
                    img.save(savepath +'/%s'%names + lastname)
                    self.s1.insert('end', '\n重新保存图片%s' %name +'1次')
                except socket.timeout:
                    img.save(savepath +'/%s'%names + lastname)
                    self.s1.insert('end', '\n重新保存图片%s' % name + '%d次'%(count+1))
                    count = count + 1
                if count >=2:
                    os.remove(savepath +'/%s'%names + lastname)
        except:
            names = name.split('.')[0]
            os.remove(savepath +'/%s'%names + lastname)


    def pre_main(self,picpath,savepath,pictype,xLen,yLen,lastname):
        nameList = self.getNameList(picpath)
        self.s1.insert('end','\n共有%d张图片需要处理'%len(nameList))
        for name in nameList:
            self.s1.insert('end', '\n正在处理图片:' + name)
            prth = threading.Thread(target=self.picdeal, args=(name,picpath,savepath,pictype,xLen,yLen,lastname,))
            prth.setDaemon(True)
            prth.start()


    
if __name__ == '__main__':
    a = Crawler_plusplus()
    a.run()
