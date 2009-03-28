/***************************************************************************
 *   Copyright 2005-2009 Last.fm Ltd.                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#ifndef LASTFM_USER_H
#define LASTFM_USER_H

#include <lastfm/WsKeys>
#include <QString>
#include <QUrl>


namespace lastfm
{
    class LASTFM_TYPES_DLLEXPORT User
    {
        QString m_name;
	
    public:    
        User( const QString& name ) : m_name( name ), m_match( -1.0f )
        {}

        operator QString() const { return m_name; }
        QString name() const { return m_name; }
	
        /** use Tag::list() on the response to get a WeightedStringList */
    	WsReply* getTopTags() const;

        /** use User::list() on the response to get a QList<User> */
        WsReply* getFriends() const;
    	WsReply* getNeighbours() const;
    
        WsReply* getPlaylists() const;
        WsReply* getTopArtists() const;
        WsReply* getRecentArtists() const;
        WsReply* getRecentTracks() const;
    
        static QList<User> list( WsReply* );
    
    //////
    	QUrl smallImageUrl() const { return m_smallImage; }
    	QUrl mediumImageUrl() const { return m_mediumImage; }
    	QUrl largeImageUrl() const { return m_largeImage; }
	
        QString realName() const { return m_realName; }
    
        /** the user's profile page at www.last.fm */
        QUrl www() const;
    
    	/** Returns the match between the logged in user and the user which this
    	  *	object represents (if < 0.0f then not set) */
    	float match() const { return m_match; }
	
    private:
    	QUrl m_smallImage;
    	QUrl m_mediumImage;
    	QUrl m_largeImage;
	
    	float m_match;
    
        QString m_realName;
    };


    /** The authenticated user is special, as some webservices only work for him */
    class LASTFM_TYPES_DLLEXPORT AuthenticatedUser : public User
    {
        using User::match; //hide as not useful
    
    public:
        /** the authenticated User */
        AuthenticatedUser() : User( Ws::Username )
        {}    

    	/** you can only get information about the autheticated user */
    	static WsReply* getInfo();
	
    	/** a verbose string, eg. "A man with 36,153 scrobbles" */
        static QString getInfoString( WsReply* );
        
        // pass the result to Artist::list(), if you want the other data 
        // you have to parse the lfm() yourself members
        // http://www.last.fm/api/show?service=388
        static WsReply* getRecommendedArtists();
    };
}

#endif
